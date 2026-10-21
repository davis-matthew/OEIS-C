/*
 * A086822  a(n) = floor((6*n^0+5*n^1+4*n^2+3*n^3) / (1*n^0+1*n^1+1*n^2)).
 * https://oeis.org/A086822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n + 1 for n ≥ 3, a(1) = 6, a(2) = 8
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)6;
    }
    if (n == 2) {
        return (seq_t)8;
    }
    /* For n ≥ 3, a(n) = 3*n + 1 */
    return (seq_t)n * (seq_t)3 + (seq_t)1;
}
