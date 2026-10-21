/*
 * A235877  Number of (n+1) X (1+1) 0..2 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * ( 2^(n+1) + 3^n ) (computed by multiplicative powers,
 *            works for any seq_t type)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)     /* offset starts at 1 – not used normally */
        return 0;

    /* compute 2^(n+1) */
    seq_t two_pow = 1;
    for (idx_t i = 0; i <= n; ++i)
        two_pow *= 2;

    /* compute 3^n */
    seq_t three_pow = 1;
    for (idx_t i = 0; i < n; ++i)
        three_pow *= 3;

    /* a(n) = 3 * (two_pow + three_pow) */
    seq_t result = (seq_t)3 * (two_pow + three_pow);
    return result;
}
