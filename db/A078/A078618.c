/*
 * A078618  a(n) = floor(average of first n cubes).
 * https://oeis.org/A078618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*(n+1)^2/4)
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the product using 128‑bit arithmetic to avoid
       intermediate overflow.  The final division by 4 gives the
       correct floor value since the expression is always an integer. */
    unsigned __int128 val = (unsigned __int128)n;
    val = val * (n + 1);          /* val = n*(n+1) */
    val = val * (n + 1);          /* val = n*(n+1)^2 */
    val /= 4;                     /* exact division by 4 */
    return (seq_t)val;
}
