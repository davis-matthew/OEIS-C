/*
 * A028340  Coefficient of x^3 in expansion of (x+1)*(x+3)*...*(x+2*n-1).
 * https://oeis.org/A028340
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^3 in ∏_{k=1..n} (x + (2k-1))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) {
        /* The sequence is defined for n ≥ 3; return 0 for smaller indices. */
        return (seq_t)0;
    }

    seq_t c0 = 1;   /* coefficient of x^0  (constant term) */
    seq_t c1 = 0;   /* coefficient of x^1  */
    seq_t c2 = 0;   /* coefficient of x^2  */
    seq_t c3 = 0;   /* coefficient of x^3  */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_k = (seq_t)(2 * k - 1);          /* current constant term (2k-1) */

        seq_t new_c3 = c2 + a_k * c3;
        seq_t new_c2 = c1 + a_k * c2;
        seq_t new_c1 = c0 + a_k * c1;
        seq_t new_c0 = a_k * c0;

        c0 = new_c0;
        c1 = new_c1;
        c2 = new_c2;
        c3 = new_c3;
    }

    return c3;
}
