/*
 * A035029  a(n) = Sum_{k=0..n} (k+1) * Sum_{j=0..n}
 *          2^j*binomial(n,j)*binomial(n-k,j).
 * https://oeis.org/A035029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binom(n,k) * binom(n+2+k, n+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the sum in the OEIS entry, this formula
   is computed in O(n) time with only integer arithmetic.  */
seq_t a(idx_t n)
{
    seq_t sum        = 0;          /* cumulative sum of the terms */
    seq_t binom_nk   = 1;          /* binom(n, k)   (kept for current k) */
    seq_t binom_two  = 1;          /* binom(n+2+k, n+2) (kept for current k) */
    seq_t term       = 1;          /* current term binom_nk * binom_two */

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += term;               /* add current term to the sum */

        if (k == n)              /* last iteration finished */
            break;

        /* update binom(n, k+1) : binom_nk = binom_nk * (n-k)/(k+1) */
        seq_t mul1 = (seq_t)(n - k);
        seq_t den1 = (seq_t)(k + 1);
        binom_nk = binom_nk * mul1 / den1;

        /* update binom(n+2+k+1, n+2) : binom_two = binom_two * (n+3+k)/(k+1) */
        seq_t mul2 = (seq_t)(n + 3 + k);
        seq_t den2 = (seq_t)(k + 1);
        binom_two = binom_two * mul2 / den2;

        /* new term for k+1 */
        term = binom_nk * binom_two;
    }

    return sum;
}
