/*
 * A143943  The Wiener index of a chain of n squares joined at vertices (i.e.,
 *          joined like <><><>...<>; here <> is a square!).
 * https://oeis.org/A143943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2 + 3*n + 3*n^2)
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1126, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;      /* cast index to the sequence type */
    seq_t ns2 = ns * ns;      /* n^2 */
    return ns * (2 + 3 * ns + 3 * ns2); /* n*(2 + 3*n + 3*n^2) */
}
