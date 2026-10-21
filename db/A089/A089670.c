/*
 * A089670  a(n) = S3(n,2), where S3(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^3.
 * https://oeis.org/A089670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^2 * (Sum_{j=0..k} binomial(n,j))^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Local variables: binom = binomial(n, k-1), Ck = Σ_{j=0..k} binomial(n,j) */
    seq_t binom = 1;      /* starts as binomial(n,0) */
    seq_t Ck    = 0;      /* cumulative binomial sum */
    seq_t sum   = 0;      /* running total of the required sum */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0)
        {
            /* For k = 0, binom = binomial(n,0) = 1 and Ck = 1 (only j = 0) */
            Ck = 1;
        }
        else
        {
            /* Update binom to binomial(n,k) using the recurrence:
               binom(n,k) = binom(n,k-1) * (n-k+1) / k          */
            binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
            Ck += binom;            /* Add the new binomial term to the cumulative sum */
        }

        /* Compute the term k^2 * Ck^3 and add to the total sum */
        seq_t kseq   = (seq_t)k;
        seq_t k2     = kseq * kseq;        /* k^2 */
        seq_t Ck2    = Ck * Ck;            /* Ck^2 */
        seq_t Ck3    = Ck2 * Ck;           /* Ck^3 */
        seq_t term   = k2 * Ck3;           /* k^2 * Ck^3 */
        sum += term;
    }

    return sum;
}
