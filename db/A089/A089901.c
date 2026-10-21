/*
 * A089901  Main diagonal of A089900, also the inverse hyperbinomial of
 *          A000312 (offset 1).
 * https://oeis.org/A089901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * binomial(n,k) * (k+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sum directly in O(n) time using only integer arithmetic. */

static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n may be 0, so handle it specially to avoid division by 0 later. */
    if (n == 0) return (seq_t)1;

    seq_t sum   = 0;
    seq_t fact  = 1;                      /* holds (k+1)!  */
    seq_t binom = 1;                      /* holds binomial(n,k) */
    seq_t pow_n = pow_seq_local((seq_t)n, n);   /* n^(n-k) with k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = n^(n-k) * C(n,k) * (k+1)! */
        sum += pow_n * binom * fact;

        if (k == n) break;               /* last iteration, no further updates */

        /* Update factorial: (k+2)! for next iteration */
        fact *= (seq_t)(k + 2);

        /* Update binomial: C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        binom *= (seq_t)(n - k);
        binom /= (seq_t)(k + 1);

        /* Update power: n^(n-(k+1)) = n^(n-k) / n */
        pow_n /= (seq_t)n;
    }

    return sum;
}
