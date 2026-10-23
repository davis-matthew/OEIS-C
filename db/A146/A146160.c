/*
 * A146160  Period 4: repeat [1, 4, 1, 16].
 * https://oeis.org/A146160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 when n % 4 == 1 or 3; a(n) = 4 when n % 4 == 2; a(n) =
 *            16 when n % 4 == 0
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute term a(n) for n >= 1 */
seq_t a(idx_t n)
{
    idx_t r = n & 3;          /* n % 4, since 4 is a power of two */
    if (r == 1 || r == 3)
        return (seq_t)1;
    if (r == 2)
        return (seq_t)4;
    /* r == 0 */
    return (seq_t)16;
}
