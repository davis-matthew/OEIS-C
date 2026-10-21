/*
 * A058895  a(n) = n^4 - n.
 * https://oeis.org/A058895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 - n
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute nn^4 - nn.  For n up to ≈10000 the intermediate
       fits in 64‑bit, and for arbitrary‑precision seq_t it is
       handled by the big integer implementation. */
    seq_t tmp = nn * nn;      /* n^2 */
    tmp = tmp * nn;           /* n^3 */
    tmp = tmp * nn;           /* n^4 */
    tmp = tmp - nn;           /* n^4 - n */
    return tmp;
}
