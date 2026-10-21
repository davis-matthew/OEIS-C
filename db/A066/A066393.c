/*
 * A066393  Coordination sequence for (9^3, 3.9^2) net with respect to a
 *          vertex of type 9^3.
 * https://oeis.org/A066393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + lcm(n,3) / 2 If n%3 == 0 the result simplifies to
 *            2*n, otherwise to 3*n. a(0) = 1
 * Range:     u8 n <= 85, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    if (nn % 3 == 0)          /* n divisible by 3: lcm(n,3)=n */
        return (seq_t)2 * nn; /* a(n) = 2*n */
    else
        return (seq_t)3 * nn; /* a(n) = 3*n */
}
