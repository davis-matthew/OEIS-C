/*
 * A187012  Antidiagonal sums of A103516.
 * https://oeis.org/A187012
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2) = 1; a(n) = n-1 for odd n; a(n) = 3n/2 - 1 for even n > 2
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)2) {
        return (seq_t)1;
    }
    if ((n & (idx_t)1)) {          /* odd n */
        return (seq_t)n - (seq_t)1;
    }
    /* even n > 2 */
    return ((seq_t)n * (seq_t)3) / (seq_t)2 - (seq_t)1;
}
