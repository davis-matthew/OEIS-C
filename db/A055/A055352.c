/*
 * A055352  Number of labeled mobiles (circular rooted trees) with n nodes and
 *          5 leaves.
 * https://oeis.org/A055352
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! *
 *            (n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(15*n^3-75*n^2+110*n-48) / 691200
 * Range:     u8 none, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 6)             /* sequence undefined for n < 6 */
        return (seq_t)0;

    /* primes and their exponents in the divisor 691200 = 2^10 * 3^3 * 5^2 */
    const idx_t primes[3] = {2, 3, 5};
    idx_t den_cnt[3] = {10, 3, 2};

    seq_t res = 1;

    /* multiply 1..n (factorial), cancel divisor factors when possible */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t mul = i;
        for (int p = 0; p < 3; ++p) {
            idx_t prime = primes[p];
            while (mul % prime == 0 && den_cnt[p] > 0) {
                mul /= prime;
                den_cnt[p]--;
            }
        }
        res *= (seq_t)mul;
    }

    /* multiply the extra factors (n-1) .. (n-5) */
    for (idx_t k = 1; k <= 5; ++k) {
        idx_t mul = n - k;
        /* no cancellation needed – denominator already exhausted */
        res *= (seq_t)mul;
    }

    /* compute the cubic polynomial part */
    seq_t nn = (seq_t)n;
    seq_t poly = (seq_t)15 * nn * nn * nn
                - (seq_t)75 * nn * nn
                + (seq_t)110 * nn
                - (seq_t)48;

    /* cancel remaining divisor factors from the polynomial if any */
    /* (it never happens for this sequence, but the code is general) */
    for (int p = 0; p < 3; ++p) {
        idx_t prime = primes[p];
        while (poly % prime == 0 && den_cnt[p] > 0) {
            poly /= prime;
            den_cnt[p]--;
        }
    }
    res *= poly;

    /* if any divisor factors remain (should not for n >= 6), divide them out */
    if (den_cnt[0] || den_cnt[1] || den_cnt[2]) {
        seq_t div = 1;
        for (int p = 0; p < 3; ++p) {
            for (idx_t c = 0; c < den_cnt[p]; ++c)
                div *= (seq_t)primes[p];
        }
        res /= div;
    }

    return res;
}
