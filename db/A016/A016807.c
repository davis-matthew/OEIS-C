/*
 * A016807  a(n) = (4*n)^7.
 * https://oeis.org/A016807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)^7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 141, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (4*n)^7 using repeated multiplication.
       Works for all provided seq_t types (uint8, ... , __int128, or arbitrary‑precision). */
    seq_t base = (seq_t)4 * n;
    seq_t res  = 1;
    for (int i = 0; i < 7; ++i)
        res *= base;
    return res;
}
