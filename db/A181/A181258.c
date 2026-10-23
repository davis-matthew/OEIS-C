/*
 * A181258  Number of (n+2)X6 binary matrices with every 3X3 block having
 *          exactly four 1's.
 * https://oeis.org/A181258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A181258 – Number of (n+2)×6 binary matrices with every 3×3 block having exactly four 1’s.
    The sequence satisfies the linear recurrence
        a(n) =  4*a(n‑1) – 3*a(n‑2) + 36*a(n‑3) – 144*a(n‑4) + 108*a(n‑5)
                – 503*a(n‑6) + 2012*a(n‑7) – 1509*a(n‑8) + 3480*a(n‑9)
                – 13920*a(n‑10) + 10440*a(n‑11) – 12564*a(n‑12)
                + 50256*a(n‑13) – 37692*a(n‑14) + 22464*a(n‑15)
                – 89856*a(n‑16) + 67392*a(n‑17) – 15552*a(n‑18)
                + 62208*a(n‑19) – 46656*a(n‑20)
    with initial values
        a(1)=2484, a(2)=3942, a(3)=6360, a(4)=10818, a(5)=22512,
        a(6)=48846, a(7)=112500, a(8)=289830, a(9)=764832,
        a(10)=2072106, a(11)=5854728, a(12)=16700214,
        a(13)=48109644, a(14)=140771982, a(15)=413533560,
        a(16)=1219726098, a(17)=3620006112, a(18)=10762255326,
        a(19)=32052060900, a(20)=95704407030.
*/

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* coefficients for the 20‑term linear recurrence */
static const int coeffs[20] = {
     4, -3,  36, -144, 108,
   -503, 2012, -1509, 3480, -13920,
   10440, -12564, 50256, -37692, 22464,
   -89856, 67392, -15552, 62208, -46656
};

/* 1‑indexed initial values; index 0 is unused (zero) */
static const seq_t init_vals[21] = {
    0ULL, 2484ULL, 3942ULL, 6360ULL, 10818ULL,
    22512ULL, 48846ULL, 112500ULL, 289830ULL, 764832ULL,
    2072106ULL, 5854728ULL, 16700214ULL, 48109644ULL,
    140771982ULL, 413533560ULL, 1219726098ULL,
    3620006112ULL, 10762255326ULL, 32052060900ULL, 95704407030ULL
};

/* cache of computed terms; cache[0] is unused */
static seq_t *cache = NULL;        /* 1‑indexed cache */
static size_t cached_size = 0;     /* number of allocated slots (index 0 unused) */

/* ------------------------------------------------------------ */
/*  Return the n‑th term of the sequence (n ≥ 1).               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is undefined for n < 1 */

    /* desired cache size: indices 0..n (0 unused) */
    size_t target = (size_t)n + 1;

    /* expand cache if required */
    if (target > cached_size) {
        seq_t *new_cache;
        if (cache == NULL) {
            /* allocate new array of size target */
            new_cache = seq_alloc(target);
        } else {
            /* reallocate to larger size */
            new_cache = seq_realloc(cache, cached_size, target);
        }
        if (!new_cache) return 0; /* allocation failure */

        cache = new_cache;

        /* initialise new cells to zero */
        for (size_t i = cached_size; i < target; ++i) cache[i] = 0;

        /* compute new values */
        for (size_t i = cached_size; i < target; ++i) {
            if (i == 0) continue;          /* skip unused index 0 */

            if (i <= 20) {                 /* initial values */
                cache[i] = init_vals[i];
            } else {                       /* recurrence */
                seq_t sum = 0;
                for (int k = 0; k < 20; ++k) {
                    seq_t term = cache[i - (k + 1)];
                    int c = coeffs[k];
                    if (c >= 0) {
                        sum += (seq_t)c * term;
                    } else {
                        sum -= (seq_t)(-c) * term;
                    }
                }
                cache[i] = sum;
            }
        }

        cached_size = target;
    }

    return cache[(size_t)n];
}
