/*
 * A021158  Decimal expansion of 1/154.
 * https://oeis.org/A021158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th digit (0-indexed) of the decimal expansion of
 *            1/154
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)1;          /* remainder before processing digit n */
    idx_t i;
    for (i = 0; i <= n; ++i)
    {
        r = r * (seq_t)10;                       /* bring next decimal place */
        seq_t d = r / (seq_t)154;                /* digit */
        r = r % (seq_t)154;                      /* new remainder */
        if (i == n) return d;                    /* return when target reached */
    }
    return (seq_t)0; /* should never happen, but keeps compiler happy */
}
