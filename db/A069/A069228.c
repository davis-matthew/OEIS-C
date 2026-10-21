/*
 * A069228  a(1)=1, a(2)=4, a(n+2)=(a(n+1)+a(n))/b(n), where
 *          b(n)=gcd(a(n+1)+a(n),4).
 * https://oeis.org/A069228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 196, u32 n <= 588, u64 n <= 1222,
 *            u128 n <= 2296, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4,
   a(n+2) = (a(n+1)+a(n))/gcd(a(n+1)+a(n),4)  */
/* The denominator is 1,2 or 4, depending on (a(n+1)+a(n)) mod 4.
   To avoid overflow when seq_t is an unsigned 64‑bit integer, the
   division by 2 or 4 is performed using the identities:
        (x+y)/2 = (x/2)+(y/2)+((x%2)+(y%2))/2
        (x+y)/4 = (x/4)+(y/4)+((x%4)+(y%4))/4        */

#include <stddef.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        /* Offset is 1; a(0) is unused but return 0 for completeness */
        return 0;
    }

    /* static cache: cache[i] holds a(i+1)  (i = 0 → a(1)) */
    static seq_t *cache = NULL;
    static size_t cap = 0;  /* allocated capacity in number of terms */
    static size_t sz  = 0;  /* number of terms currently filled */

    /* Grow the cache if needed. Keep old cache to maintain statelessness. */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : (size_t)n;
        if (new_cap < (size_t)n) new_cap = (size_t)n; /* safety */

        seq_t *new_cache = seq_alloc(new_cap);
        for (size_t i = 0; i < sz; ++i) {
            new_cache[i] = cache[i];
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing terms up to a(n). */
    for (size_t i = sz; i < (size_t)n; ++i) {
        if (i == 0) {                /* a(1) */
            cache[i] = (seq_t)1;
        } else if (i == 1) {         /* a(2) */
            cache[i] = (seq_t)4;
        } else {
            seq_t a_prev  = cache[i - 1];
            seq_t a_prev2 = cache[i - 2];

            /* Compute denominator via (a+b) mod 4 */
            unsigned int a_mod4 = (unsigned int)(a_prev & 3);
            unsigned int b_mod4 = (unsigned int)(a_prev2 & 3);
            unsigned int sum_mod4 = (a_mod4 + b_mod4) & 3;

            unsigned int denom;
            if (sum_mod4 == 0)         denom = 4;
            else if (sum_mod4 == 2)    denom = 2;
            else                       denom = 1;

            seq_t sum = a_prev + a_prev2;

            /* Detect overflow for unsigned 64‑bit type when denom > 1 */
            int overflow = 0;
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
            overflow = (sizeof(seq_t) <= 8) && (denom > 1) && (sum < a_prev);
#else
            overflow = 0;
#endif

            seq_t result;
            if (!overflow) {
                if (denom == 1) {
                    result = sum;
                } else {
                    result = sum / (seq_t)denom;
                }
            } else {
                /* Overflow happened; denom is 2 or 4. Use identities to avoid overflow. */
                if (denom == 2) {
                    seq_t a_half    = a_prev / 2;
                    seq_t b_half    = a_prev2 / 2;
                    seq_t rem       = (a_prev % 2) + (a_prev2 % 2);
                    seq_t add       = rem / 2;   /* rem is 0,1,2 → add is 0 or 1 */
                    result = a_half + b_half + add;
                } else /* denom == 4 */ {
                    seq_t a_quarter = a_prev / 4;
                    seq_t b_quarter = a_prev2 / 4;
                    seq_t rem       = (a_prev % 4) + (a_prev2 % 4);
                    seq_t add       = rem / 4;   /* rem is 0..6 → add is 0 or 1 */
                    result = a_quarter + b_quarter + add;
                }
            }

            cache[i] = result;
        }
    }
    sz = (size_t)n;

    return cache[n - 1];
}
