/*
 * A212039  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any element within a city block distance of two, and containing
 *          the value n(n+1)/2-2.
 * https://oeis.org/A212039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2*n^3 - 25*n^2 + 46*n - 16) / 8 for n > 1, with
 *            a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */

    /* Compute the numerator in a single unsigned addition/subtraction
       sequence that avoids underflow on unsigned types. */
    seq_t total = n4 + (2 * n3) + (46 * nn);
    total -= (25 * n2);
    total -= (seq_t)16;

    return total / (seq_t)8;
}
