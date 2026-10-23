/*
 * A120990  a(n) = 5^n - 3^n - 2*n.
 * https://oeis.org/A120990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 3^n - 2*n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 5^n and 3^n by repeated multiplication */
    seq_t pow5 = 1;
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
        pow3 *= (seq_t)3;
    }

    /* 2*n as seq_t */
    seq_t twon = (seq_t)2 * (seq_t)n;

    /* a(n) = 5^n - 3^n - 2*n */
    seq_t result = pow5 - pow3;
    result -= twon;
    return result;
}
