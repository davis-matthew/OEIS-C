/*
 * A144685  Size of acyclic domain of size n based on the alternating scheme.
 * https://oeis.org/A144685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3)*(n+3) – binomial_term where if n even:
 *            binomial_term = binomial(n-2, n/2-1) * (2*n-3) / 2 if n odd:
 *            binomial_term = binomial(n-1, (n-1)/2) * (n-1) / 2 The first
 *            three terms are hard‑coded because the formula uses negative
 *            exponents for n < 4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* first few values are special cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* base part: 2^(n-3)*(n+3) */
    seq_t term = pow2(n - 3);
    term = term * (seq_t)(n + 3);

    /* binomial correction */
    seq_t b, corr;
    if ((n & 1ULL) == 0) {                 /* n even */
        b = binomial(n - 2, n / 2 - 1);
        corr = b * (seq_t)(2 * n - 3);
        corr >>= 1;                       /* divide by 2 */
    } else {                               /* n odd */
        b = binomial(n - 1, (n - 1) / 2);
        corr = b * (seq_t)(n - 1);
        corr >>= 1;                       /* divide by 2 */
    }

    return term - corr;
}
