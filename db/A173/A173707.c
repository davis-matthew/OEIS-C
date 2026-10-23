/*
 * A173707  Partial sums of floor(n^3/3).
 * https://oeis.org/A173707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^4 + 2*n^3 + n^2 - 4*n)/12)
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 475, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate so the calculation is safe for all
       n that fit in idx_t (up to 10^4 and beyond).  The result
       fits into seq_t because the division by 12 reduces the size. */
    unsigned __int128 nn = n;
    unsigned __int128 value =
        nn * nn * nn * nn          /* n^4   */
      + 2 * nn * nn * nn          /* 2 n^3 */
      + nn * nn                   /* n^2   */
      - 4 * nn;                   /* -4 n  */
    value /= 12;                  /* floor division */

    return (seq_t)value;
}
