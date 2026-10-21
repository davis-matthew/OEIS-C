/*
 * A094622  Expansion of x*(11+20*x)/((1-x)*(1-10*x^2)).
 * https://oeis.org/A094622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0)=0; a(n)=1 followed by ((n-1)/2) copies of 4 then 1 if n is
 *            odd; a(n)=3 followed by (n/2-1) copies of 4 then 1 if n is even
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) {
        return 0U;
    }

    seq_t val;
    if ((n & 1U) != 0U) {                /* odd n */
        idx_t m = (n - 1U) / 2U;         /* number of 4's */
        val = 1U;
        for (idx_t i = 1U; i <= m; ++i) {
            val = val * 10U + 4U;
        }
        val = val * 10U + 1U;
    } else {                              /* even n */
        idx_t m = n / 2U;                 /* half index */
        val = 3U;
        for (idx_t i = 1U; i <= m - 1U; ++i) {
            val = val * 10U + 4U;
        }
        val = val * 10U + 1U;
    }
    return val;
}
