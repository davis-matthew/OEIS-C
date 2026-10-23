/*
 * A139544  Numbers which are not the difference of two squares of positive
 *          integers.
 * https://oeis.org/A139544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 10 for n >= 4; a(1) = 1; a(2) = 2; a(3) = 4
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    seq_t res = (seq_t)n;
    res = 4 * res - 10;   /* Works for any seq_t that can hold 4*n-10 */
    return res;
}
