/*
 * A067424  Ninth column of triangle A067417.
 * https://oeis.org/A067424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, n = 0 a(n) = 11 * 27^(n-1), n ≥ 1 (A067424)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;       /* first term */

    /* Compute 27^(n-1) by repeated multiplication */
    seq_t pow = 1;
    for (idx_t i = 1; i < n; ++i)
        pow *= (seq_t)27;      /* multiply by 27, n-1 times */

    return (seq_t)11 * pow;    /* final term */
}
