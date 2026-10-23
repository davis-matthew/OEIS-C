/*
 * A172067  Expansion of (2/(3*sqrt(1-4*z)-1+4*z))*((1-sqrt(1-4*z))/(2*z))^k
 *          with k=10.
 * https://oeis.org/A172067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j*binomial(2*n+10-j,n-j) A recurrence
 *            is used: 2*n*(n+10)*(3*n+17)*a(n) -
 *            (21*n^3+317*n^2+1622*n+2880)*a(n-1) -
 *            2*(3*n+20)*(n+4)*(2*n+9)*a(n-2) = 0 Initial values: a(0)=1,
 *            a(1)=11 The implementation memoises computed terms. 128‑bit
 *            arithmetic is used for 64‑bit seq_t to a...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 7, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* elements stored so far */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)n) /* guard against overflow of shift */
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)11;
        } else {
            /* use 128‑bit arith when seq_t is 64‑bit */
            if (sizeof(seq_t) == 8) {
                /* 128‑bit integer type (extension, available on GCC/Clang) */
                typedef unsigned __int128 uint128;
                uint128 sn  = (uint128)i;
                uint128 coeff1 = 21*sn*sn*sn
                                + 317*sn*sn
                                + 1622*sn
                                + 2880;
                uint128 coeff2 = 2 * (3*sn + 20) * (sn + 4) * (2*sn + 9);
                uint128 denom  = 2*sn*(sn+10)*(3*sn+17);

                uint128 term1 = coeff1 * (uint128)cache[i-1];
                uint128 term2 = coeff2 * (uint128)cache[i-2];
                uint128 num   = term1 + term2;

                cache[i] = (seq_t)(num / denom);
            } else {
                /* generic seq_t arithmetic – works for arbitrary‑precision types */
                seq_t sn   = (seq_t)i;
                seq_t coeff1 = 0;
                coeff1 = coeff1 + (seq_t)(21) * sn * sn * sn;
                coeff1 = coeff1 + (seq_t)(317) * sn * sn;
                coeff1 = coeff1 + (seq_t)(1622) * sn;
                coeff1 = coeff1 + (seq_t)(2880);

                seq_t coeff2 = (seq_t)(2) * (seq_t)(3*sn + 20)
                                 * (sn + (seq_t)(4))
                                 * ((seq_t)(2*sn + 9));

                seq_t denom = (seq_t)(2) * sn * (sn + (seq_t)(10))
                               * (seq_t)(3*sn + 17);

                seq_t num = coeff1 * cache[i-1] + coeff2 * cache[i-2];
                cache[i]  = num / denom;
            }
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
