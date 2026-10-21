/*
 * A202832  E.g.f: exp(2*x + 5*x^2/2).
 * https://oeis.org/A202832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 2;
   a(n) = 2*a(n-1) + 5*(n-1)*a(n-2) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)2;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* cur = 2*prev1 + 5*(i-1)*prev2 */
        cur = prev1 * 2;

        seq_t mult = (seq_t)(i - 1);
        mult *= (seq_t)5;
        cur += mult * prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
