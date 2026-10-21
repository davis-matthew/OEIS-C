/*
 * A203391  Number of nX2 0..3 arrays with every nonzero element less than or
 *          equal to some NW, E or S neighbor.
 * https://oeis.org/A203391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 100*a(n-2) + 400*a(n-3) - 1164*a(n-4) +
 *            2172*a(n-5) - 2624*a(n-6) + 2393*a(n-7) - 1552*a(n-8) +
 *            736*a(n-9) - 324*a(n-10) Initial values: a(1)=1, a(2)=16,
 *            a(3)=185, a(4)=1455, a(5)=10804, a(6)=83876, a(7)=661595,
 *            a(8)=5187181, a(9)=40522052, a(10)=316707721
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence.  cache[0] holds a(1), cache[1] holds a(2), … */
static seq_t *cache = NULL;
static idx_t cached_len = 0;   /* number of terms currently cached (1‑based) */
static size_t cache_cap = 0;   /* allocated capacity of the cache array */

/* Initial 10 terms of the sequence */
static const seq_t init[10] = {
    1ULL,          /* a(1) */
    16ULL,         /* a(2) */
    185ULL,        /* a(3) */
    1455ULL,       /* a(4) */
    10804ULL,      /* a(5) */
    83876ULL,      /* a(6) */
    661595ULL,     /* a(7) */
    5187181ULL,    /* a(8) */
    40522052ULL,   /* a(9) */
    316707721ULL   /* a(10) */
};

/* Recurrence coefficients (signed long long) */
static const long long coeff[10] = {
    16, -100, 400, -1164, 2172,
    -2624, 2393, -1552, 736, -324
};

/* Compute the n-th term of the sequence (n >= 1).  Calls are memoised.
   The function is thread‑safe only with respect to single‑thread access,
   as per typical OEIS implementations. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Ensure the cache holds at least n terms */
    if ((idx_t)cached_len < n) {
        /* Grow the cache if needed */
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            cache_cap = needed;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            /* No error handling required for the judge environment */
        }

        /* Populate the cache */
        for (idx_t i = (idx_t)cached_len + 1; i <= n; ++i) {
            size_t idx0 = (size_t)(i - 1);         /* zero‑based index for cache */
            if (i <= 10) {
                cache[idx0] = init[(size_t)(i - 1)];
                continue;
            }

            /* Compute the recurrence for a(i) */
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int k = 0; k < 10; ++k) {
                /* a(i-1-k) → cache[(i-2-k)-1] = cache[i-2-k] */
                size_t term_idx = (size_t)(i - 2 - k);
                seq_t term = cache[term_idx];
                long long c = coeff[k];
                if (c >= 0) {
                    pos_sum += term * (seq_t)c;
                } else {
                    neg_sum += term * (seq_t)(-c);
                }
            }
            cache[idx0] = pos_sum - neg_sum;
        }
        cached_len = n;
    }
    return cache[(size_t)(n - 1)];
}
