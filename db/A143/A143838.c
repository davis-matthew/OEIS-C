/*
 * A143838  Ulam's spiral (SSW spoke).
 * https://oeis.org/A143838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 27*n + 12
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = 16 * nn * nn;
    t -= 27 * nn;
    t += 12;
    return t;
}
