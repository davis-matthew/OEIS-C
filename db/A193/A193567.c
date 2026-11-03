/*
 * A193567  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 37.
 * https://oeis.org/A193567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A193567.
   A(n) is the nth positive integer that is unhappy and whose first
   member of the cycle (4, 16, 37, 58, 89, 145, 42, 20) reached during
   iteration of the sum‑of‑squares‑of‑digits function is 37. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Sum of the squares of the decimal digits of x. */
static seq_t next_digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        seq_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

/* Returns true iff starting from x the first element of the cycle
   reached during iteration is 37. */
static bool enters_cycle_at_37(seq_t x)
{
    /* Elements of the unhappy cycle. */
    const seq_t cycle[] = {4, 16, 37, 58, 89, 145, 42, 20};
    const size_t cycle_len = sizeof(cycle) / sizeof(cycle[0]);

    while (true) {
        /* If x is a member of the cycle, we have reached the cycle. */
        for (size_t i = 0; i < cycle_len; ++i) {
            if (x == cycle[i])
                return x == 37;
        }
        /* Happy numbers terminate at 1. */
        if (x == 1)
            return false;
        x = next_digit_square_sum(x);
    }
}

/* Memoised generation of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static seq_t candidate = 1;   /* Number to test next. */

    while (cache_size < n) {
        if (enters_cycle_at_37(candidate)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap + cache_cap : 64;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_size++] = candidate;
        }
        candidate++;
    }
    return cache[n - 1];
}
