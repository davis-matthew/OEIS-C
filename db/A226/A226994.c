/*
 * A226994  Number of lattice paths from (0,0) to (n,n) consisting of steps
 *          U=(1,1), H=(1,0) and S=(0,1) such that the first step leaving the
 *          diagonal (if any) is an H step.
 * https://oeis.org/A226994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k)^2 * 2^(k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A226994, offset 0) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    seq_t sum = (seq_t)1;   /* term for k = 0 (empty path) */
    seq_t bin = (seq_t)1;   /* binomial(n,0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update bin to binomial(n,k) using multiplicative recurrence.
           The intermediate product and division are exact for integer
           (big‑int) arithmetic.   */
        bin = (bin * ((seq_t)n - (k - 1))) / (k);

        /* bin^2 */
        seq_t bin_sq = bin * bin;

        /* 2^(k-1) : left‑shift works for all seq_t types
           (fixed‑width or arbitrary‑precision).          */
        seq_t pow2k = ((seq_t)1) << (k - 1);

        /* Accumulate the term */
        sum += bin_sq * pow2k;
    }

    return sum;
}
