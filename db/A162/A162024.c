/*
 * A162024  Number of n X n binary arrays with rows and columns, considered as
 *          binary numbers, in nondecreasing order, and all but the outermost
 *          row or column zero.
 * https://oeis.org/A162024
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n*n - 8*n + 8 for n > 2, a(2) = 7
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)7;
    seq_t sn = (seq_t)n;
    return ((seq_t)4 * sn * sn) - ((seq_t)8 * sn) + (seq_t)8;
}
