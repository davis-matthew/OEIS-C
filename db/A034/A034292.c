/*
 * A034292  Fibonacci numbers with exactly 2 different digits (probably
 *          finite).
 * https://oeis.org/A034292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-03)
 * Helpers:   digit_counts, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fibonacci numbers with exactly two distinct decimal digits.
   a(n) = nth Fibonacci number that contains exactly two different digits. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static bool is_two_digit(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    int distinct = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d])
            ++distinct;
    return distinct == 2;
}

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static seq_t f_prev = 0;      /* Fibonacci F(0) */
    static seq_t f_curr = 1;      /* Fibonacci F(1) */
    static idx_t fib_index = 1;   /* index of f_curr (currently 1) */

    /* generate Fibonacci numbers until we have n+1 qualifying terms */
    while (cache_len <= (size_t)n) {
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
        ++fib_index;              /* f_curr now holds F_{fib_index} */

        if (is_two_digit(f_curr))
            append_candidate(&cache, &cache_len, &cache_cap, f_curr);
    }

    return cache[(size_t)n];
}
