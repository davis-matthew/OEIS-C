/*
 * A055815  a(n) = T(2*n+3,n), array T as in A055807.
 * https://oeis.org/A055815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{s=1}^{n+3} binom(n+3,s) * binom(s+n-2,n-1) (n≥1),
 *            a(0) = 1. This uses only integer arithmetic on seq_t values
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t total = n + 3;          /* upper limit of the sum (s=1..n+3) */
    seq_t sum   = 0;              /* running total of the sum                */
    seq_t term1 = 1;              /* binom(total, 0) initially                */
    seq_t term2 = 1;              /* binom(n-1, n-1) = 1 initially           */

    for (idx_t s = 1; s <= total; ++s) {
        /* term1 becomes binom(total, s) */
        term1 = term1 * (total - (s - 1)) / s;

        /* add current product to the sum */
        sum += term1 * term2;

        /* update term2 to binom(s+n-2, n-1) for the next iteration */
        /* term2_next = term2 * (s + n - 1) / s  (exact division) */
        term2 = term2 * (s + n - 1) / s;
    }

    return sum;
}
