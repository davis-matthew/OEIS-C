/*
 * A184633  Floor(1/{(9+n^4)^(1/4)}), where {} = fractional part.
 * https://oeis.org/A184633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4 * n^3 / 9) for n >= 3; the first two values are
 *            a(1) = 1, a(2) = 4
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1504, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special cases directly */
    if (n == 1) return 1;
    if (n == 2) return 4;

    /* For n >= 3 the formula holds.  All operations are performed
       using seq_t to avoid any issues with other integer widths. */
    seq_t x  = (seq_t)n;          /* x = n  */
    seq_t val = x * x * x * 4;    /* val = 4 * n^3 */
    return val / 9;               /* floor(4 n^3 / 9) */
}
