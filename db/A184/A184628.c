/*
 * A184628  Floor(1/frac((4+n^4)^(1/4))), where frac(x) is the fractional part
 *          of x.
 * https://oeis.org/A184628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n >= 2, a(n) = n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)2;
    }
    seq_t nn = (seq_t)n;
    return nn * nn * nn;
}
