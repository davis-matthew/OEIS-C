/*
 * A157669  a(n) = 19683*n - 5967.
 * https://oeis.org/A157669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19683*n - 5967
 * Range:     u8 none, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t before multiplication to avoid
       unintended integer promotion. */
    return ((seq_t)19683 * n) - (seq_t)5967;
}
