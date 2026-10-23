/*
 * A181547  a(n) = Sum_{k=0..floor(n/2)} C(n-k,k)^5.
 * https://oeis.org/A181547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)^5 (OEIS A181547).
 *            The implementation uses the recurrence B(k)=C(n-k,k) derived
 *            from B(k)=B(k‑1)*(n-2k+2)*(n-2k+1)/(k*(n-k+1)) to avoid
 *            recomputing factorials. All arithmetic is performed in the
 *            generic integer type seq_t
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)0;
    idx_t maxk  = n >> 1;          /* floor(n/2) */
    seq_t prev  = (seq_t)1;        /* B(0)=1 */

    /* k = 0 term */
    seq_t pk = prev * prev * prev * prev * prev; /* prev^5 == 1^5 */
    sum += pk;

    for (idx_t k = 1; k <= maxk; ++k) {
        /* Compute B(k) from B(k-1) */
        seq_t nmk2 = (seq_t)n - (seq_t)(2*k) + (seq_t)2; /* n-2k+2 */
        seq_t nmk1 = (seq_t)n - (seq_t)(2*k) + (seq_t)1; /* n-2k+1 */
        seq_t denom1 = (seq_t)k;
        seq_t denom2 = (seq_t)n - (seq_t)k + (seq_t)1;   /* n-k+1 */

        seq_t numerator   = nmk2 * nmk1;
        seq_t denominator = denom1 * denom2;

        seq_t cur = (prev * numerator) / denominator; /* exact division */
        prev = cur;

        /* cur^5 by repeated multiplication */
        pk = cur;
        pk *= cur;
        pk *= cur;
        pk *= cur;
        pk *= cur;

        sum += pk;
    }
    return sum;
}
