/*
 * A082045  Diagonal sums of number array A082043.
 * https://oeis.org/A082045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(n^4 - n^3 + 11*n^2 - 11*n + 30)/30
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow before the final division */
    unsigned __int128 t = n;                                     /* cast index to 128‑bit */
    unsigned __int128 num = (t + 1) *
        (t*t*t*t - t*t*t + 11*t*t - 11*t + 30);                 /* numerator */
    return (seq_t)(num / 30);                                   /* exact division by 30 */
}
