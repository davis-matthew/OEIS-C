/*
 * A215943  Number of ways to place k non-attacking bishops on an n x n
 *          toroidal chessboard, summed over all k >= 0.
 * https://oeis.org/A215943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (for all n ≥ 1):
   If n is odd:
        a(n) = Σ_{k=0..n} C(n,k)^2 * k!
   If n is even, write n = 2m:
        a(n) = ( Σ_{k=0..m} C(m,k)^2 * k! * 2^k )^2    */
seq_t a(idx_t n)
{
    /* n is at least 1 */
    if (n & 1) { /* odd n */
        idx_t limit = n;
        seq_t binom = 1;      /* C(n,0) */
        seq_t fact  = 1;      /* 0! */
        seq_t sum   = 0;

        for (idx_t k = 0; k <= limit; ++k) {
            seq_t term = binom * binom * fact;
            sum += term;

            if (k == limit) break;      /* finish loop */

            /* update binom to C(n,k+1) and fact to (k+1)! */
            binom = binom * ((seq_t)(n - k)) / ((seq_t)(k + 1));
            fact  = fact * ((seq_t)(k + 1));
        }

        return sum;
    } else {                /* even n */
        idx_t m = n >> 1;   /* n/2 */
        seq_t binom = 1;      /* C(m,0) */
        seq_t fact  = 1;      /* 0! */
        seq_t pow2  = 1;      /* 2^0 */
        seq_t sum   = 0;

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = binom * binom * fact * pow2;
            sum += term;

            if (k == m) break;      /* finish loop */

            /* update binom to C(m,k+1), fact to (k+1)! and pow2 to 2^(k+1) */
            binom = binom * ((seq_t)(m - k)) / ((seq_t)(k + 1));
            fact  = fact * ((seq_t)(k + 1));
            pow2  = pow2 * (seq_t)2;
        }

        /* a(2m) = (Σ ...)^2 */
        return sum * sum;
    }
}
