/*
 * A082642  Expansion of Molien series for 5-dimensional representation of
 *          dihedral group of order 10.
 * https://oeis.org/A082642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-5) – a(n-7) – a(n-8) – a(n-9) +
 *            a(n-12) + N(n) where N(0)=1, N(2)=1, N(3)=1, N(4)=2, N(5)=2,
 *            N(6)=2, N(7)=1, N(8)=1, N(10)=1, and N(n)=0 for all other n.
 *            This recurrence comes from the rational generating function
 *            (1+x²+x³+2x⁴+2x⁵+2x⁶+x⁷+x⁸+x¹⁰) /*
 * Range:     u8 n <= 49, u16 n <= 808, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* coefficients that have been computed */
static idx_t cache_len = 0;     /* number of entries currently stored (>=1 if any) */

/* sequence a(n) for n ≥ 0 */
seq_t a(idx_t n)
{
    /* Extend the cached array to include index n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (new_cache == NULL) {            /* allocation failed */
            return 0;
        }
        cache = new_cache;

        /* Phenomenon: for i < cache_len old values are already correct.
           For i ≥ cache_len old values are zero (not used), but we will
           compute each new entry in increasing order.                 */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;
            /* Positive contributions */
            if (i >= 3) val += cache[i - 3];
            if (i >= 4) val += cache[i - 4];
            if (i >= 5) val += cache[i - 5];
            if (i >= 12) val += cache[i - 12];
            /* Negative contributions */
            if (i >= 7) val -= cache[i - 7];
            if (i >= 8) val -= cache[i - 8];
            if (i >= 9) val -= cache[i - 9];
            /* Add the numerator term N(i) */
            if (i <= 12) {
                /* 0‑based array of N(i), i=0..12.  Elements >12 are zero. */
                static const seq_t N[13] = {
                    1, /* N(0) */
                    0, /* N(1) */
                    1, /* N(2) */
                    1, /* N(3) */
                    2, /* N(4) */
                    2, /* N(5) */
                    2, /* N(6) */
                    1, /* N(7) */
                    1, /* N(8) */
                    0, /* N(9) */
                    1, /* N(10) */
                    0, /* N(11) */
                    0  /* N(12) */
                };
                val += N[i];
            }
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
