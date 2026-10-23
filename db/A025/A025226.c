/*
 * A025226  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 2.
 *          Also a(n) = 3^n*C(n-1), where C = A000108 (Catalan numbers).
 * https://oeis.org/A025226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * A000108(n-1) = (3^n/(n)) * binomial(2n-2, n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(n) = a(1)*a(n-1)+a(2)*a(n-2)+...+a(n-1)*a(1)  (n>=2)  */

#include "../utils/pow3.h"    /* seq_t pow3(idx_t exp); */
#include "../utils/binomial.h" /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for offset 1 but safe */
    if (n == 1) return (seq_t)3;      /* a(1) = 3 */

    /* a(n) = 3^n * binomial(2n-2, n-1) / n  */
    idx_t two_n_minus_2 = 2 * n - 2;   /* safe in idx_t */
    seq_t bin = binomial(two_n_minus_2, n - 1);
    seq_t bin_div = bin / (seq_t)n;    /* division exact */
    seq_t pow3_n = pow3(n);
    return pow3_n * bin_div;
}
