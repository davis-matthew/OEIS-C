/*
 * A068548  Coefficients of (-x^(2n-6)) in Chebyshev polynomial of degree 2n.
 * https://oeis.org/A068548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2*n^2+3*n-2)/12*4^n (equivalently a(n) =
 *            (n*(2n^2+3n-2)/3)*4^(n-1) )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    seq_t ns = (seq_t)n;
    /* compute the integer part n*(2n^2+3n-2) */
    seq_t term = ns *
        (((seq_t)2 * ns * ns) + ((seq_t)3 * ns) - (seq_t)2);

    /* divide by 3 first – the product is always divisible by 3 */
    seq_t part = term / (seq_t)3;

    /* compute 4^(n-1) */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow4 *= (seq_t)4;
    }

    /* final value */
    return part * pow4;
}
