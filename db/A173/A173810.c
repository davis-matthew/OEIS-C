/*
 * A173810  a(n) = (8*10^n - 71)/9 for n > 0.
 * https://oeis.org/A173810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 71 for n>1, a(1) = 1 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* not used for defined offset */

    /* start at a(1) */
    seq_t val = 1;

    /* compute recursively: a(k) = 10*a(k-1) + 71 */
    for (idx_t i = 2; i <= n; ++i)
        val = 10 * val + 71;

    return val;
}
