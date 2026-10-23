/*
 * A168510  Products across consecutive rows of the denominators of the
 *          Leibniz harmonic triangle (A003506).
 * https://oeis.org/A168510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*Product_{k=1..n} k^(2k-n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   build_spf, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   /* build_spf */
#include "../utils/seq_one.h"    /* seq_one */
#include "../utils/seq_mul.h"    /* seq_mul */
#include "../utils/seq_pow.h"    /* seq_pow */

seq_t a(idx_t n)
{
    if (n <= 1) return seq_one();               /* a(1)=1 */

    /* spf[1] is unused, spf[i] gives the smallest prime factor of i   */
    idx_t *spf = NULL;
    build_spf(n, &spf);                         /* spf[2..n] top-structure */

    /* exponent of each prime up to n, signed 64 bits */
    int64_t *exps = (int64_t *)calloc((size_t)n + 1, sizeof(int64_t));
    if (!exps) {
        free(spf);
        return seq_one();
    }

    /* accumulate prime exponents from n! and from k^(2k-n-1) */
    for (idx_t k = 2; k <= n; ++k) {
        int64_t ek = 2LL * (int64_t)k - (int64_t)n - 1;   /* e_k = 2k-n-1 */

        idx_t tmp = k;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            int   cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            /* factor k contributes (1 + e_k) copies of each prime factor */
            int64_t add = (ek + 1) * (int64_t)cnt;
            exps[p] += add;
        }
    }

    /* build the product of prime powers */
    seq_t result = seq_one();
    for (idx_t p = 2; p <= n; ++p) {
        int64_t exp = exps[p];
        if (exp > 0) {
            seq_t power = seq_pow((seq_t)p, (idx_t)exp);
            result = seq_mul(result, power);
        }
    }

    free(exps);
    free(spf);
    return result;
}
