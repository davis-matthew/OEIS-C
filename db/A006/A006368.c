/*
 * A006368  The "amusical permutation" of the nonnegative numbers: a(2n) = 3n,
 *          a(4n+1) = 3n+1, a(4n-1) = 3n-1.
 * https://oeis.org/A006368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   if n even, then a(n) = 3*n/2, otherwise a(n) = round(3*n/4) =
 *            (3*n + 2) / 4
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 1) == 0) {
        return ((seq_t)n * 3) / 2;
    } else {
        return ((seq_t)n * 3 + 2) / 4;
    }
}
