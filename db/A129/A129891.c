/*
 * A129891  Sum of coefficients of polynomials defined in comments lines.
 * https://oeis.org/A129891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) − 2*a(n-2) + a(n-4) (n ≥ 4) Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=9 Sequence A129891 – Sum of
 *            coefficients of certain polynomials
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized implementation using a dynamically resizable array. */
static seq_t *cache = NULL;     /* stored sequence values               */
static size_t  ca_size = 0;     /* number of stored terms               */
static size_t  ca_cap   = 0;    /* allocated array capacity             */

/* Utility to grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= ca_cap) return;
    size_t cap = ca_cap > 0 ? ca_cap * 2 : 4;
    if (cap < new_cap) cap = new_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real library one would handle the error more gracefully. */
        abort();
    }
    cache = new_cache;
    ca_cap = cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache at least contains values up to n. */
    if (ca_size <= (size_t)n) {
        /* Grow capacity and initialize missing terms. */
        double new_size_f = (double)(n) + 1.0;
        size_t new_size = (size_t) (new_size_f + 0.5);
        grow_cache(new_size);

        /* Initialize base cases if this is the first call. */
        if (ca_size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)9;
            ca_size = 4;
        }

        /* Fill remaining terms using the recurrence. */
        for (size_t i = ca_size; i <= (size_t)n; ++i) {
            /* For i == 0..3 we already have the values. */
            if (i < 4) continue;
            seq_t val = (seq_t)3 * cache[i - 1]
                        - (seq_t)2 * cache[i - 2]
                        + cache[i - 4];
            cache[i] = val;
        }
        ca_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
