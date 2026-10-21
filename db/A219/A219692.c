/*
 * A219692  a(n) = Sum_{j=0..floor(n/3)} (-1)^j C(n,j) * C(2j,j) *
 *          C(2n-2j,n-j) * (C(2n-3j-1,n) + C(2n-3j,n)).
 * https://oeis.org/A219692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..floor(n/3)} (-1)^j * C(n,j) * C(2j,j) *
 *            C(2n-2j, n-j) * (C(2n-3j-1, n) + C(2n-3j, n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;          /* a(0) = 2 */

    idx_t maxj = n / 3;
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t sign = (j & 1) ? 0 : 1;      /* 1 if j even, 0 if j odd */

        seq_t t1 = binomial(n, j);
        seq_t t2 = binomial(2 * j, j);
        seq_t t3 = binomial(2 * n - 2 * j, n - j);

        seq_t t4a = binomial(2 * n - 3 * j - 1, n);
        seq_t t4b = binomial(2 * n - 3 * j, n);
        seq_t t4  = t4a + t4b;

        seq_t term = t1 * t2;
        term = term * t3;
        term = term * t4;

        if (sign) {          /* j even → add to positive sum */
            sum_pos += term;
        } else {              /* j odd  → add to negative sum */
            sum_neg += term;
        }
    }

    /* Final result is positive; pos ≥ neg for all n. */
    return sum_pos - sum_neg;
}
