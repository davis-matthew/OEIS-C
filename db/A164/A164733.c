/*
 * A164733  Number of n-digit fixed points under the Kaprekar map A151949.
 * https://oeis.org/A164733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164733.
 *
 *  a(n) is defined by the rational generating function
 *
 *        x*(-x^32 + x^31 - x^29 + x^28 - x^27 + x^26 - x^24 + 2*x^23
 *           - x^22 + x^21 + x^20 + 2*x^18 - x^17 + x^16 + 2*x^15
 *           - 3*x^14 + 2*x^13 - x^12 + x^11 - x^9 + 2*x^8 - x^6 + x^5
 *           - x^4 + x^3 + 1)
 *
 *        -----------------------------------
 *        x^31 - x^29 - x^25 + x^23 - x^22 + x^20 - x^17 + x^16 + x^15
 *        - x^14 + x^11 - x^9 + x^8 - x^6 - x^2 + 1
 *
 *  From this we obtain the linear recurrence
 *
 *        a(n) = B[n] - Σ_{k=1}^{31} d_k * a(n-k)          (n ≥ 1)
 *
 *  where the coefficients d_k are the coefficients of the denominator
 *  (with the constant term omitted) and B[n] are the coefficients of the
 *  numerator (zero for n > 33).  The first few terms are
 *
 *        a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=0, a(6)=2, a(7)=0, …
 *
 *  The coefficients d_k (k = 1 … 31) are:
 *
 *        d[1] = 0
 *        d[2] = -1
 *        d[3] = 0
 *        d[4] = 0
 *        d[5] = 0
 *        d[6] = -1
 *        d[7] = 0
 *        d[8] = 1
 *        d[9] = -1
 *        d[10] = 0
 *        d[11] = 1
 *        d[12] = 0
 *        d[13] = 0
 *        d[14] = -1
 *        d[15] = 1
 *        d[16] = 1
 *        d[17] = -1
 *        d[18] = 0
 *        d[19] = 0
 *        d[20] = 1
 *        d[21] = 0
 *        d[22] = -1
 *        d[23] = 1
 *        d[24] = 0
 *        d[25] = -1
 *        d[26] = 0
 *        d[27] = 0
 *        d[28] = 0
 *        d[29] = -1
 *        d[30] = 0
 *        d[31] = 1
 *
 *  The numerator coefficients B[n] for n = 0 … 33 (zero for n = 0) are:
 *
 *        B[ 0] =  0,  B[ 1] =  1,  B[ 2] =  0,  B[ 3] =  0,
 *        B[ 4] =  1,  B[ 5] = -1,  B[ 6] =  1,  B[ 7] = -1,
 *        B[ 8] =  0,  B[ 9] =  2,  B[10] = -1,  B[11] =  0,
 *        B[12] =  1,  B[13] = -1,  B[14] =  2,  B[15] = -3,
 *        B[16] =  2,  B[17] =  1,  B[18] = -1,  B[19] =  2,
 *        B[20] =  0,  B[21] =  1,  B[22] =  1,  B[23] = -1,
 *        B[24] =  2,  B[25] = -1,  B[26] =  0,  B[27] =  1,
 *        B[28] = -1,  B[29] =  1,  B[30] = -1,  B[31] =  0,
 *        B[32] =  1,  B[33] = -1
 *
 *  The implementation below memoises the values in a static cache.
 */

#include <stdlib.h>
#include <string.h>

/* coefficients of the denominator, 1 ≤ k ≤ 31 */
static const int d[32] = {
    /* 0 unused */
    0,   /* d[0] placeholder */
    0,   /* d[1] */
   -1,   /* d[2] */
    0,   /* d[3] */
    0,   /* d[4] */
    0,   /* d[5] */
   -1,   /* d[6] */
    0,   /* d[7] */
    1,   /* d[8] */
   -1,   /* d[9] */
    0,   /* d[10] */
    1,   /* d[11] */
    0,   /* d[12] */
    0,   /* d[13] */
   -1,   /* d[14] */
    1,   /* d[15] */
    1,   /* d[16] */
   -1,   /* d[17] */
    0,   /* d[18] */
    0,   /* d[19] */
    1,   /* d[20] */
    0,   /* d[21] */
   -1,   /* d[22] */
    1,   /* d[23] */
    0,   /* d[24] */
   -1,   /* d[25] */
    0,   /* d[26] */
    0,   /* d[27] */
    0,   /* d[28] */
   -1,   /* d[29] */
    0,   /* d[30] */
    1    /* d[31] */
};

/* coefficients of the numerator, 0 ≤ n ≤ 33 */
static const int B[34] = {
    0,  /* B[0] */
    1,  /* B[1] */
    0,  /* B[2] */
    0,  /* B[3] */
    1,  /* B[4] */
   -1,  /* B[5] */
    1,  /* B[6] */
   -1,  /* B[7] */
    0,  /* B[8] */
    2,  /* B[9] */
   -1,  /* B[10] */
    0,  /* B[11] */
    1,  /* B[12] */
   -1,  /* B[13] */
    2,  /* B[14] */
   -3,  /* B[15] */
    2,  /* B[16] */
    1,  /* B[17] */
   -1,  /* B[18] */
    2,  /* B[19] */
    0,  /* B[20] */
    1,  /* B[21] */
    1,  /* B[22] */
   -1,  /* B[23] */
    2,  /* B[24] */
   -1,  /* B[25] */
    0,  /* B[26] */
    1,  /* B[27] */
   -1,  /* B[28] */
    1,  /* B[29] */
   -1,  /* B[30] */
    0,  /* B[31] */
    1,  /* B[32] */
   -1   /* B[33] */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated length */

    if (n == 0) return 0;            /* not used, but safe */

    /* Allocate or extend the cache */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }

        /* Compute new entries */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            if (i == 0) { new_cache[i] = 0; continue; }

            __int128 sum = 0;
            for (int k = 1; k <= 31 && (size_t)k <= i; ++k) {
                if (d[k] == 0) continue;
                sum += (__int128)d[k] * (__int128)new_cache[i - k];
            }

            int bi = 0;
            if (i <= 33) bi = B[i];

            __int128 ai = (__int128)bi - sum;
            new_cache[i] = (seq_t)ai;
        }

        cache = new_cache;
        cap = new_cap;
    }

    return cache[n];
}
