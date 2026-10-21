/*
 * A099748  Molien series for complete weight enumerators of Euclidean
 *          self-dual codes over the Galois ring GR(4,2) that contain the
 *          all-ones vector.
 * https://oeis.org/A099748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^(4*n) in (f(t^4)+t^156*f(t^-4)) / *
 *            ((1-t^4)^2*(1-t^8)^5*(1-t^12)^5*(1-t^16)^4), * where
 *            f(t)=1+13*t^2+180*t^3+2815*t^4+26097*t^5+178909*t^6 *
 *            +914207*t^7+3741734*t^8+12638167*t^9+36300248*t^10 *
 *            +90284593*t^11+197531731*t^12+384302984*t^13+670899605*t^14 *
 *            +1057839714*t^1...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to exponent in the power series */
    size_t exp = (size_t)n << 2;          /* exp = 4*n */

    /* Constants: degrees */
    enum { DEG_B = 172, DEG_A = 156 };

    /* Static storage for precomputed data */
    static long long B[DEG_B + 1];
    static seq_t  A[DEG_A + 1];
    static int    init = 0;

    /* Build B and A once */
    if (!init) {
        /* Initialize B (denominator polynomial) */
        for (int i = 0; i <= DEG_B; ++i) B[i] = 0;
        const long long c1[3]  = { 1, -2, 1 };
        const int        e1[3] = { 0, 4, 8 };
        const long long c2[6]  = { 1, -5, 10, -10, 5, -1 };
        const int        e2[6] = { 0, 8, 16, 24, 32, 40 };
        const long long c3[6]  = { 1, -5, 10, -10, 5, -1 };
        const int        e3[6] = { 0, 12, 24, 36, 48, 60 };
        const long long c4[5]  = { 1, -4, 6, -4, 1 };
        const int        e4[5] = { 0, 16, 32, 48, 64 };

        for (int i1 = 0; i1 < 3; ++i1)
            for (int i2 = 0; i2 < 6; ++i2)
                for (int i3 = 0; i3 < 6; ++i3)
                    for (int i4 = 0; i4 < 5; ++i4) {
                        int d = e1[i1] + e2[i2] + e3[i3] + e4[i4];
                        long long v = c1[i1] * c2[i2] * c3[i3] * c4[i4];
                        B[d] += v;
                    }

        /* Initialize A (numerator polynomial) */
        for (int i = 0; i <= DEG_A; ++i) A[i] = 0;

        static const seq_t f[20] = {
            1,                           /* t^0 */
            0,                           /* t^1 : absent */
            13,                          /* t^2 */
            180,                         /* t^3 */
            2815,                        /* t^4 */
            26097,                       /* t^5 */
            178909,                      /* t^6 */
            914207,                      /* t^7 */
            3741734,                     /* t^8 */
            12638167,                    /* t^9 */
            36300248,                    /* t^10 */
            90284593,                    /* t^11 */
            197531731,                   /* t^12 */
            384302984,                   /* t^13 */
            670899605,                   /* t^14 */
            1057839714,                  /* t^15 */
            1514398168,                  /* t^16 */
            1975782327,                  /* t^17 */
            2355691682,                  /* t^18 */
            2571102793                   /* t^19 */
        };

        /* f(t^4) part */
        for (int k = 0; k <= 19; ++k) {
            seq_t coeff = f[k];
            int d = 4 * k;
            if (d <= DEG_A) A[d] = coeff;
        }
        /* t^156 * f(t^-4) part */
        for (int k = 0; k <= 19; ++k) {
            seq_t coeff = f[k];
            int d = 156 - 4 * k;
            if (d <= DEG_A) A[d] = coeff;
        }
        init = 1;
    }

    /* Static cache for already computed series coefficients */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */

    /* Allocate initial cache if necessary */
    if (!cache) {
        cache_len = 0;
        cache = (seq_t*)malloc((size_t)(1000) * sizeof(seq_t)); /* arbitrary start */
        if (!cache) abort();
    }

    /* Compute all coefficients up to required exponent */
    while (cache_len <= exp) {
        size_t k = cache_len;
        seq_t val = (k <= DEG_A) ? A[k] : 0;

        /* Linear recurrence from denominator: b0=1 */
        for (int i = 1; i <= DEG_B && (size_t)i <= k; ++i) {
            long long bi = B[i];
            if (!bi) continue;
            seq_t prod = cache[k - i];
            if (bi > 0)
                val -= prod * (seq_t)bi;
            else
                val += prod * (seq_t)(-bi);
        }

        /* Ensure enough space in cache */
        if (cache_len == (size_t)(0)) { /* not needed but guard */
            /* nothing */
        }
        if (cache_len == (size_t)(-1)) { /* avoid overflow */
            /* nothing */
        }

        cache[cache_len++] = val;
    }

    return cache[exp];
}
