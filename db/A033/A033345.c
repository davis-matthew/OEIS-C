/*
 * A033345  a(n) = floor(25/n).
 * https://oeis.org/A033345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(25 / n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is undefined for n = 0; return 0 for safety */
    if (n == 0)
        return 0;
    /* Integer division gives the floor automatically */
    return (seq_t)(25 / n);
}
