/*
 * A096229  0*9, 1*8, 2*7, 3*6, 4*5; 10*19, 11*18, ..., 14*15; 20*29, 21*28,
 *          ..., 24*25; 30*39, ...
 * https://oeis.org/A096229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (floor(n/5)*10 + (n mod 5)) * (floor(n/5)*10 + 9 - (n mod
 *            5))
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = floor(n/5) and r = n mod 5 */
    seq_t m = (seq_t)(n / 5);          /* m fits in seq_t because it is <= n */
    seq_t r = (seq_t)(n % 5);

    /* common base = m * 10 */
    seq_t base = m * 10;

    /* first factor = base + r */
    seq_t first = base + r;

    /* second factor = base + 9 - r */
    seq_t second = base + 9 - r;

    /* product */
    return first * second;
}
