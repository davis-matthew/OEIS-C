/*
 * A229422  Number of n X 2 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, rows
 *          lexicographically nondecreasing, and columns lexicographically
 *          nonincreasing.
 * https://oeis.org/A229422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 10*n^3 + 11*n^2 + 50*n + 48) / 24
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type */
    seq_t sn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = sn * sn;      /* n^2 */
    seq_t n3 = n2 * sn;      /* n^3 */
    seq_t n4 = n3 * sn;      /* n^4 */

    /* Compute the numerator of the rational expression */
    seq_t num = n4
              + ((seq_t)10) * n3
              + ((seq_t)11) * n2
              + ((seq_t)50) * sn
              + ((seq_t)48);

    /* Divide by 24 – the division is exact for all integer n */
    return num / (seq_t)24;
}
