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
 * Helpers:   enters_cycle_at_37, next_digit_square_sum
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
#include "../utils/enters_cycle_at_37.h"
#include "../utils/next_digit_square_sum.h"

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
                seq_t *tmp = (seq_t *)realloc(cache,
                               cache_cap * sizeof(*cache));
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
