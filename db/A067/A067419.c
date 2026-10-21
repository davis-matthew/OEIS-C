/*
 * A067419  Fourth column of triangle A067417.
 * https://oeis.org/A067419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 6 * 12^(n-1) for n >= 1 (equivalently a(n) = 12
 *            * a(n-1), n > 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 12^(n-1) iteratively */
    seq_t pow12 = (seq_t)1;
    idx_t exp = n - 1;
    for (idx_t i = 0; i < exp; ++i)
        pow12 *= (seq_t)12;

    /* Multiply by 6 to obtain the term */
    return (seq_t)6 * pow12;
}
