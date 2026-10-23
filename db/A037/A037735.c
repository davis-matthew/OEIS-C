/*
 * A037735  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0,3.
 * https://oeis.org/A037735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037735: Base‑9 digits are, in order, the first n terms of
   the periodic sequence with initial period 2,1,0,3.
   a(1) = 2
   a(n) = 9*a(n-1) + d_n,   d_n = 2,1,0,3,2,1,0,3,… (period 4). */

#include <stdlib.h>

/* Cache for previously computed terms. */
static seq_t *cache = NULL;   /* dynamically growable array, 1‑indexed */
static idx_t cache_len = 0;   /* number of terms stored */

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but harmless */

    /* Extend the cache if we haven't reached n yet. */
    if (n > cache_len) {
        /* Allocate space for indices 0 … n (index 0 unused). */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort(); /* insufficient memory; terminate naturally */
        cache = tmp;

        /* Determine where to start computing. */
        idx_t start = cache_len ? cache_len + 1 : 1;

        /* Initialize the first term if it hasn't been set. */
        if (cache_len == 0) {
            cache[1] = 2;
            start = 2;
        }

        /* Predefined repeating digits: 2,1,0,3. */
        static const int pat[4] = { 2, 1, 0, 3 };

        /* Compute terms from 'start' up to n. */
        for (idx_t i = start; i <= n; ++i) {
            int digit = pat[(i - 1) & 3];     /* (i-1) % 4 using bit‑mask */
            cache[i] = cache[i - 1] * 9 + (seq_t)digit;
        }

        cache_len = n;
    }

    return cache[n];
}
