/*
 * A074494  Number of 2-input gates used to synthesize parity function in
 *          disjunctive normal form (DNF) with n inputs.
 * https://oeis.org/A074494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n * 2^(n-2) - 1 for n > 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;          /* a(1) = 2 */

    /* compute 3*n first */
    seq_t res = (seq_t)3 * (seq_t)n;       /* (3*n) in seq_t */

    /* multiply by 2^(n-2) */
    idx_t i;
    for (i = 0; i < n - 2; ++i)
        res <<= 1;                         /* res *= 2 */

    /* subtract 1 */
    res -= (seq_t)1;

    return res;
}
