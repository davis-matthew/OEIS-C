/*
 * A016905  a(n) = (5*n + 4)^9.
 * https://oeis.org/A016905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 26, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (5*n + 4) as seq_t */
    seq_t base = ((seq_t)5 * (seq_t)n) + ((seq_t)4);
    /* Multiply base by itself 9 times */
    seq_t r = 1;
    for (int i = 0; i < 9; ++i)
        r *= base;
    return r;
}
