/*
 * A062013  a(1) = 3, a(n) = a(n-1)^2 + 1.
 * https://oeis.org/A062013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3, a(n) = a(n-1)^2 + 1 (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)   /* safeguard against accidental 0 */
        return (seq_t)0;

    seq_t val = (seq_t)3;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val = val * val + (seq_t)1; /* a(i) = a(i-1)^2 + 1 */

    return val;
}
