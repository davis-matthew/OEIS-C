/*
 * A173294  Values of 16*n^2+24*n+7, n>=0, each duplicated.
 * https://oeis.org/A173294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(floor(n/2))^2 + 24*floor(n/2) + 7
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each value is duplicated: a(2k)=a(2k+1) = 16k^2 + 24k + 7 */
seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n / 2);              /* floor division */
    return (seq_t)16 * k * k + (seq_t)24 * k + (seq_t)7;
}
