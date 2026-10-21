/*
 * A092183  Figurate numbers based on the 120-cell (4-D polytope with
 *          Schlaefli symbol {5,3,3}).
 * https://oeis.org/A092183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(261*n^3 - 504*n^2 + 283*n - 38)/2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote n to the sequence type first */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */

    /* Compute the polynomial part */
    seq_t part = 261 * n3;            /* 261*n^3 */
    part -= 504 * n2;                /* -504*n^2 */
    part += 283 * nn;                /* +283*n  */
    part -= 38;                      /* -38     */

    /* Final multiplication and division by 2 */
    return (nn * part) / 2;
}
