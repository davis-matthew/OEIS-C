/*
 * A089381  L-th order palindromes with L > 2.
 * https://oeis.org/A089381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th integer > 0 that eventually cycles under the
 *            rule P(m) = (m/2) if m even, else m + rev_decimal(m), with a
 *            cycle length L > 2
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   ensure_capacity, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/rev_decimal.h"
#include "../utils/ensure_capacity.h"

/* Helper: iterate P on a number */
static inline seq_t P(seq_t m)
{
    /* m % 2 == 0  => even */
    if ((m % 2) == 0)
        return m / 2;
    else
        /* odd: m + rev(m) */
        return m + rev_decimal(m);
}

/* Detect the cycle length of the orbit of start.
   Return the cycle length; 0 if the cycle length is 2 or less. */
static int cycle_len(seq_t start)
{
    seq_t tortoise = P(start);
    seq_t hare     = P(P(start));

    /* Find a repetition point. */
    while (tortoise != hare) {
        tortoise = P(tortoise);
        hare     = P(P(hare));
    }

    /* Find the start of the cycle. */
    seq_t t = start;
    seq_t h = tortoise;
    while (t != h) {
        t = P(t);
        h = P(h);
    }

    /* Count the cycle length. */
    int lambda = 1;
    seq_t next = P(t);
    while (next != t) {
        next = P(next);
        ++lambda;
    }
    return lambda;
}

/* Static state to memoise results. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_scan  = 1;

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n elements. */
    while (cache_len < n) {
        int L = cycle_len((seq_t)next_scan);
        if (L > 2) {
            /* Append next_scan to cache. */
            if (cache_len >= cache_cap) {
                /* Grow capacity at least double. */
                size_t needed = cache_cap ? cache_cap * 2 : 16;
                ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_scan;
        }
        ++next_scan;
    }
    return cache[n - 1];
}
