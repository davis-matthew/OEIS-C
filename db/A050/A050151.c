/*
 * A050151  a(n) = T(n,n+2), array T as in A050143.
 * https://oeis.org/A050151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} 2^{k-1} * C(n, k) * C(n, k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A050151. */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    if (n == 0)
        return 0;

    /* C(n,0) = 1, C(n,1) = n. */
    seq_t prev_binom = 1;          /* C(n, k-1) during the loop */
    seq_t cur_binom  = (seq_t)n;   /* C(n, k) during the loop */
    seq_t pow2 = 1;                /* 2^{k-1} during the loop */

    for (idx_t k = 1; k <= n; ++k) {
        /* term = 2^{k-1} * C(n, k) * C(n, k-1) */
        sum += pow2 * cur_binom * prev_binom;

        if (k == n)                 /* last iteration finished */
            break;

        /* prepare values for the next k+1 */
        prev_binom = cur_binom;
        /* next binomial: C(n, k+1) = C(n, k) * (n-k) / (k+1) */
        cur_binom = (cur_binom * (seq_t)(n - k)) / (seq_t)(k + 1);
        pow2 *= 2;                  /* next power of two */
    }

    return sum;
}
