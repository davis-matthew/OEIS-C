/*
 * A161701  a(n) = (n^5 - 5*n^4 + 5*n^3 + 5*n^2 + 114*n + 120)/120.
 * https://oeis.org/A161701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,0) + C(n,1) + C(n,4) + C(n,5)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 220, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_simple(idx_t n, idx_t k)
{
    /* Computes binomial coefficient C(n,k) using multiplicative formula.
       Returns 0 for k > n.  All intermediate values fit in seq_t
       for the n ranges used in the tests. */
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;   /* use symmetry to keep k small */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute the sequence term using the combinatorial formula. */
    seq_t result = 0;
    result += binom_simple(n, 0);   /* always 1 */
    result += binom_simple(n, 1);
    result += binom_simple(n, 4);
    result += binom_simple(n, 5);
    return result;
}
