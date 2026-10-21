/*
 * A211784  a(n) = n^2 + floor(n^2/2) + floor(n^2/3).
 * https://oeis.org/A211784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + floor(n^2/2) + floor(n^2/3)
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, return 0 as safe guard */
    seq_t nn = (seq_t)n * (seq_t)n;    /* compute n^2 in seq_t */
    seq_t res = nn;                    /* start with n^2 */
    res += nn / 2;                     /* add floor(n^2/2) */
    res += nn / 3;                     /* add floor(n^2/3) */
    return res;
}
