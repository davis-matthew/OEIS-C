/*
 * A117067  Decimal value of binary number whose n-th 1 is followed by F(n)
 *          0's.
 * https://oeis.org/A117067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 1; a(1) = 5; a(2) = 21; for n >= 3: a(n) = 1 + a(n-1) *
 *            (a(n-1)-1) * (a(n-2)-1) / (2 * a(n-2) * a(n-3))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)5;
    if (n == (idx_t)2) return (seq_t)21;

    /* keep last three computed terms: a[i-3], a[i-2], a[i-1] */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)5;  /* a(1) */
    seq_t a2 = (seq_t)21; /* a(2) */
    seq_t a_cur = a2;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        /* numerator factors: a2, a2-1, a1-1 */
        seq_t num[3];
        num[0] = a2;
        num[1] = a2 - (seq_t)1;
        num[2] = a1 - (seq_t)1;

        /* denominator factors: 2, a1, a0 */
        seq_t den[3];
        den[0] = (seq_t)2;
        den[1] = a1;
        den[2] = a0;

        /* cancel common factors to avoid overflow */
        for (int m = 0; m < 3; ++m) {
            for (int k = 0; k < 3; ++k) {
                if (den[k] == (seq_t)1) continue;
                seq_t g = gcd(num[m], den[k]);
                if (g != (seq_t)1) {
                    num[m] /= g;
                    den[k] /= g;
                }
            }
        }

        /* After cancellations all denominators should be 1 */
        seq_t prod = (seq_t)1;
        for (int m = 0; m < 3; ++m) {
            prod *= num[m];
        }
        a_cur = (seq_t)1 + prod;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a_cur;
    }

    return a_cur;
}
