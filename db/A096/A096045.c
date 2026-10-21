/*
 * A096045  a(n) = B(2*n, 2)/B(2*n) (see formula section).
 * https://oeis.org/A096045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^n - 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;
    seq_t base = 4;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            pow = pow * base;
        }
        exp >>= 1;
        if (exp > 0) {
            base = base * base;
        }
    }
    return pow * 3 - 2;
}
