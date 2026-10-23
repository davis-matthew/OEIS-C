/*
 * A181237  Number of (3n) X 3 binary matrices with all row sums equal and all
 *          column sums equal.
 * https://oeis.org/A181237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (1 + (3*n)!/(n!)³)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(n,k) using a multiplicative
   algorithm that works for any integer type seq_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    /* Use symmetry to reduce the number of iterations */
    if (k > n - k) k = n - k;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* res = res * (n - k + i) / i; exact division */
        res = (res * (n - k + i)) / i;
    }
    return res;
}

/* Sequence A181237: number of (3n)×3 binary matrices with all row sums equal
   and all column sums equal. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n < 1, return 0 */

    idx_t three_n = n * (idx_t)3;
    idx_t two_n   = n * (idx_t)2;

    /* (3n)!/(n!)³ = C(3n,n) * C(2n,n) */
    seq_t binom3n_n = binom(three_n, n);
    seq_t binom2n_n = binom(two_n, n);
    seq_t prod = binom3n_n * binom2n_n;

    seq_t result = ((seq_t)2) * (((seq_t)1) + prod);
    return result;
}
