/*
 * A047202  Numbers that are congruent to {2, 3, 4} mod 5.
 * https://oeis.org/A047202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - floor((n-1)/3) - ((n-1) mod 3)
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: n >= 1 */
    idx_t nm1 = n - (idx_t)1;
    idx_t div3 = nm1 / (idx_t)3;
    idx_t mod3 = nm1 % (idx_t)3;
    /* compute in seq_t to avoid overflow on idx_t multiplication */
    seq_t result = ((seq_t)n * (seq_t)2) - (seq_t)div3 - (seq_t)mod3;
    return result;
}
