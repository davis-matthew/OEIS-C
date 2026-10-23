/*
 * A139742  a(n) = 11^n - 4^n.
 * https://oeis.org/A139742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 4^n Computation: iterative multiplicative
 *            exponentiation by a small base. The code works for any unsigned
 *            integer seq_t (uint8_t … uint128) and for arbitrary-precision
 *            seq_t types provided by the C++ build
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow11 = 1;
    seq_t pow4  = 1;

    /* Compute 11^n by repeated multiplication */
    for (idx_t i = 0; i < n; ++i) {
        pow11 *= 11;
    }

    /* Compute 4^n by repeated multiplication */
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= 4;
    }

    return pow11 - pow4;
}
