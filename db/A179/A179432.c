/*
 * A179432  a(n) = C(2*3^(n-1), n).
 * https://oeis.org/A179432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2*3^(n-1), n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   gcd, mul_seq_safe, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/gcd.h"
#include "../utils/mul_seq_safe.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    /* N = 2 * 3^(n-1) */
    seq_t N = pow3(n - 1u);
    N = mul_seq_safe(N, (seq_t)2);

    seq_t res = (seq_t)1;

    for (idx_t k = 1u; k <= n; ++k) {
        seq_t numer = N - (seq_t)(k - 1u);   /* N - (k-1) */
        seq_t denom = (seq_t)k;

        /* cancel common factors between numer and denom */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        if (denom != (seq_t)1) {
            /* further cancel any remaining denominator with the current result */
            g = gcd(res, denom);
            res /= g;
            denom /= g;
        }

        /* denom should now be 1 */
        res = mul_seq_safe(res, numer);
    }

    return res;
}
