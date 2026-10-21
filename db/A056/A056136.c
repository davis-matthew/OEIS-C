/*
 * A056136  Largest positive integer whose harmonic mean with another positive
 *          integer is n.
 * https://oeis.org/A056136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 if n is odd, * a(n) = n*(n+2)/4 if n is even
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1U) { /* odd */
        /* a(n) = n*(n+1)/2 */
        return ((seq_t)n * ((seq_t)n + 1)) / 2;
    } else { /* even */
        /* a(n) = n*(n+2)/4 */
        return ((seq_t)n * ((seq_t)n + 2)) / 4;
    }
}
