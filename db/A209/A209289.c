/*
 * A209289  Number of functions f:{1,2,...,2n}->{1,2,...,2n} such that every
 *          preimage has an even cardinality.
 * https://oeis.org/A209289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(n) = ∑_{i=0}^{2n} (n-i)^(2n) * binomial(2n,i)
   The implementation uses symmetry to sum only for i = 0 … n‑1
   and doubles the result.  The term i = n vanishes for n > 0. */

/* Helper: fast exponentiation for arbitrary precision integers.
   Exponent must fit in idx_t (it does for the intended range). */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1)
            result = result * cur;
        cur = cur * cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Base case – the empty set has even cardinality. */
    if (n == 0)
        return 1;

    idx_t two_n = n * 2;

    seq_t ans = 0;
    seq_t binom = 1; /* C(2n,0) */

    for (idx_t i = 0; i < n; ++i)
    {
        /* base = n - i  (positive in this range) */
        seq_t base = (seq_t)(n - i);

        /* (n-i)^(2n) */
        seq_t powv = pow_uint(base, two_n);

        /* term = binom * powv */
        seq_t term = binom * powv;

        ans += term;

        /* Update binomial coefficient to C(2n, i+1) */
        /* binom = binom * (2n - i) / (i + 1)   (exact division) */
        binom = binom * (two_n - i) / (i + 1);
    }

    /* Double the sum to account for the symmetric i > n terms. */
    ans = ans * 2;

    return ans;
}
