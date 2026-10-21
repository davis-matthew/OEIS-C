/*
 * A095894  a(2n) = 6*n^2 + 7*n + 1; a(2n+1) = 6*n^2 + 13*n + 7.
 * https://oeis.org/A095894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 6*n^2 + 7*n + 1; a(2n+1) = 6*n^2 + 13*n + 7
 * Range:     u8 n <= 11, u16 n <= 207, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t kn = (seq_t)n;          /* promote index to seq_t */
    if ((n & 1) == 0) {          /* even index: a(2k) */
        seq_t k = kn >> 1;        /* k = n/2 */
        return 6 * k * k + 7 * k + 1;
    } else {                     /* odd index: a(2k+1) */
        seq_t k = (kn - 1) >> 1; /* k = (n-1)/2 */
        return 6 * k * k + 13 * k + 7;
    }
}
