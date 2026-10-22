/*
 * A005131  A generalized continued fraction for Euler's number e.
 * https://oeis.org/A005131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   if n ≡ 1 (mod 3), then a(n) = 2*(n-1)/3, otherwise a(n) = 1
 * Range:     u8 n <= 303, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 3 == 1) {
        seq_t sn = (seq_t)n;
        return 2 * (sn - 1) / 3;
    } else {
        return 1;
    }
}
