/*
 * A081345  First row in maze arrangement of natural numbers A081344.
 * https://oeis.org/A081345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + n + 1 - n*(-1)^n
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (n%2==0) ? n^2+1 : (n+1)^2 */
seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        seq_t nn = (seq_t)n * (seq_t)n;
        return nn + 1;
    } else {
        seq_t m = (seq_t)n + 1;
        return m * m;
    }
}
