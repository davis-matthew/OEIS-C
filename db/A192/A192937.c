/*
 * A192937  a(n) = 100*a(n-1) - (n-1) with a(1)=100.
 * https://oeis.org/A192937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) - (n-1), a(1)=100
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* undefined for n < 1 */

    const seq_t base = (seq_t)100;           /* 100 in seq_t form */
    seq_t val = (seq_t)100;                  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)           /* iterate to n */
    {
        val = base * val - (seq_t)(i - 1);    /* recurrence */
    }

    return val;
}
