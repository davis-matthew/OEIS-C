/*
 * A157843  a(n) = 1728000*n - 1343760.
 * https://oeis.org/A157843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1728000*n - 1343760
 * Range:     u8 none, u16 none, u32 n <= 2486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is defined for n ≥ 1 */
    seq_t un = (seq_t)n;              /* convert index to seq_t */
    return un * 1728000u - 1343760u; /* compute the linear expression */
}
