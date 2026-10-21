/*
 * A051503  a(n) = min { n, floor(100/n) }.
 * https://oeis.org/A051503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(n, floor(100/n))
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 100 / n using integer division gives floor(100/n) */
    idx_t div = 100 / n;
    /* choose the smaller of n and that quotient */
    idx_t res = (n < div) ? n : div;
    return (seq_t)res;
}
