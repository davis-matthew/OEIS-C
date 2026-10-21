/*
 * A069517  a(n) = (-1)*Sum_{d|n} (moebius(d)*(-1)^d).
 * https://oeis.org/A069517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = -∑_{d|n} μ(d)(-1)^d ; equivalently a(1)=1, a(2^e)=2,
 *            otherwise 0
 * Range:     u8 n <= 9631, u16 n <= 9631, u32 n <= 9631, u64 n <= 9631,
 *            u128 n <= 9631, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if ((n & (n - 1)) == 0) return 2;
    return 0;
}
