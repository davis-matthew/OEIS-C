/*
 * A160864  64*P_9(n), 64 times the Legendre polynomial of order 9 at n.
 * https://oeis.org/A160864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(315 - 4620*n^2 + 18018*n^4 - 25740*n^6 + 12155*n^8)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 52, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n6 = n4 * n2;          /* n^6 */
    seq_t n8 = n4 * n4;          /* n^8 */

    seq_t term = 315
                 - 4620 * n2
                 + 18018 * n4
                 - 25740 * n6
                 + 12155 * n8;

    if ((n & 1) == 0)            /* n even: (n/2) * term */
        return (nn >> 1) * term;
    else                         /* n odd : n * (term/2) */
        return nn * (term >> 1);
}
