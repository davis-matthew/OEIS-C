/*
 * A159721  Number of permutations of 3 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159721
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*4^(n-2) (for n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                      /* undefined for n < 2 */

    /* compute 4^(n-2) */
    seq_t pow = 1;
    idx_t e = n - 2;
    for (idx_t i = 0; i < e; ++i)
        pow *= 4;

    /* multiply by 3*n */
    seq_t factor = (seq_t)3 * (seq_t)n;
    pow *= factor;
    return pow;
}
