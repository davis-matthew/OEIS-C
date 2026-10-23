/*
 * A108366  L(n,n), where L is defined as in A108299.
 * https://oeis.org/A108366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,2k)*(n-2)^k (OEIS A108366)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle trivial cases where base (n-2) would be negative or zero */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t result = 0;
    seq_t binom   = 1;             /* binomial(n+0,0) = 1 for k=0 */
    seq_t pow     = 1;             /* (n-2)^0 = 1          */
    seq_t base    = (seq_t)(n - 2);/* base is non‑negative here */

    for (idx_t k = 0; k <= n; ++k) {
        result += binom * pow;     /* accumulate term k */

        if (k == n) break;        /* finished */

        /* Update binom for next k (k+1) */
        idx_t k1 = k + 1;
        seq_t num1 = (seq_t)(n + k1);        /* n + (k+1)   */
        seq_t num2 = (seq_t)(n - k1 + 1);    /* n - (k+1) + 1 */
        seq_t den  = ((seq_t)(2 * k1 - 1)) * (seq_t)(2 * k1); /* (2k+1)*(2k+2) */

        /* exact division – the result is an integer */
        binom = (binom * num1 * num2) / den;

        /* update power of base for next term */
        pow *= base;
    }
    return result;
}
