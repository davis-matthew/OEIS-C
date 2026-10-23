/*
 * A152919  a(1)=1, for n>1, a(n) = n^2/4 + n/2 for even n, a(n) = n^2/4 + n -
 *          5/4 for odd n.
 * https://oeis.org/A152919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n) = n^2/4 + n/2 if n is even, a(n) = n^2/4 +
 *            n – 5/4 if n is odd
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented using integer arithmetic: a(n) = (n^2 + 2n)/4 for even n,
   a(n) = (n^2 + 4n – 5)/4 for odd n. */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    if ((n & 1) == 0) {                           /* even n */
        seq_t nn = (seq_t)n * (seq_t)n;
        return (nn + 2 * (seq_t)n) / 4;
    } else {                                       /* odd n */
        seq_t nn = (seq_t)n * (seq_t)n;
        return (nn + 4 * (seq_t)n - 5) / 4;
    }
}
