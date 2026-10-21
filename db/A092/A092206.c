/*
 * A092206  Positive integers that are not of the form n^2 or 3n^2.
 * https://oeis.org/A092206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092206: numbers that are not of the form n^2 or 3*n^2.
   Computed by scanning natural numbers and discarding perfect squares
   and numbers equal to 3 times a perfect square. */

#include <stdlib.h>
#include <stdbool.h>

/* --- Small integer square‑root helper ---------------------------------- */
/* Return ⌊√x⌋ for a 64‑bit unsigned integer.  No external header used. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32;          /* > √(2⁶⁴-1)   */
    idx_t mid, res = 0;

    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid * mid <= x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Return the n‑th term of the sequence A092206 (1‑based index).        */
seq_t a(idx_t n)
{
    /* Static cache of already generated terms */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;  /* capacity in elements    */
    static idx_t cache_len   = 0;  /* number of terms stored */
    static idx_t next_m      = 2;  /* next integer to test    */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have at least n terms */
    while (cache_len < n) {
        idx_t m = next_m;
        bool excluded = false;

        /* Check whether m is a perfect square */
        idx_t t = isqrt_u64(m);
        if (t * t == m) {
            excluded = true;
        } else if (m % 3 == 0) {                /* check for 3 × square */
            idx_t u = isqrt_u64(m / 3);
            if (u * u == m / 3) {
                excluded = true;
            }
        }

        if (!excluded) {
            /* Store m in the cache, enlarging if necessary */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len] = (seq_t)m;
            cache_len++;
        }

        next_m++;
    }

    return cache[n - 1];
}
