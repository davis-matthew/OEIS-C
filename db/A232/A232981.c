/*
 * A232981  The Gauss factorial n_5!.
 * https://oeis.org/A232981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{1 <= j <= n, gcd(j,5)=1} j Recurrence: a(0)=1;
 *            a(n)=a(n-1) if n%5==0 else n*a(n-1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        if (i % 5 == 0)              /* skip multiples of 5 */
            continue;
        val = val * (seq_t)i;        /* multiply by current index */
    }
    return val;
}
