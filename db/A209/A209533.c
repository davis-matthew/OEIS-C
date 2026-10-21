/*
 * A209533  Half the number of (n+1)X8 0..2 arrays with every 2X2 subblock
 *          having exactly two distinct clockwise edge differences.
 * https://oeis.org/A209533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^(n+1)+1
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 16^(n+1) by repeated multiplication */
    seq_t result = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        result *= (seq_t)16;
    /* add 1 to match the OEIS definition */
    return result + 1;
}
