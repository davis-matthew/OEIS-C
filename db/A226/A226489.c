/*
 * A226489  a(n) = n*(15*n-11)/2.
 * https://oeis.org/A226489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15*n-11)/2
 * Range:     u8 n <= 6, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    seq_t term = (seq_t)15 * sn - 11;
    /* divide numerator by 2 before multiplication to avoid overflow */
    if ((n & 1) == 0)      /* n even */
        sn /= 2;
    else                   /* n odd */
        term /= 2;
    return sn * term;
}
