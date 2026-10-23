/*
 * A115400  Number of n-colorings of the octahedral graph.
 * https://oeis.org/A115400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^3 - 9*n^2 + 29*n - 32)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 42, u64 n <= 1627,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0, 1, 2 the value is 0 */
    if (n < 3) return 0;

    seq_t nn = (seq_t)n;

    /* Compute the factor n*(n-1)*(n-2) */
    seq_t part = nn * (nn - 1) * (nn - 2);

    /* Compute the cubic factor safely without intermediate underflow:
       (n^3 + 29*n) - 9*n^2 - 32  */
    seq_t d = (nn * nn * nn + (seq_t)29 * nn) - (seq_t)9 * nn * nn;
    d -= (seq_t)32;

    return part * d;
}
