/*
 * A229620  Incorrect version of A045949.
 * https://oeis.org/A229620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1-(-1)^n-8*n+18*n^2+12*n^3)/4
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;            /* cast index to sequence type */
    seq_t n2 = n_t * n_t;            /* n^2 */
    seq_t n3 = n2 * n_t;             /* n^3 */

    /* Combine terms: 12*n^3 + 18*n^2 */
    seq_t base = 12ULL * n3 + 18ULL * n2;

    /* Subtract 8*n */
    seq_t sub = 8ULL * n_t;

    /* Add parity contribution: 2 if n is odd, 0 if even */
    seq_t parity = (n & 1ULL) ? (seq_t)2 : (seq_t)0;

    seq_t sum = base - sub + parity;

    /* Integer division by 4 yields the desired term */
    return sum / 4;
}
