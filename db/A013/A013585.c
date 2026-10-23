/*
 * A013585  Smallest m such that 1!+...+m! is divisible by 2n+1, or 0 if no
 *          such m exists.
 * https://oeis.org/A013585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m≥1 such that Σ_{k=1}^{m} k! ≡ 0 (mod (2n+1)),
 *            else 0
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0: 2n+1 = 1, every number is divisible by 1. The smallest
       positive m with 1! + … + m! ≡ 0 (mod 1) is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Modulus 2n+1 fits in seq_t for all n used in the tests. */
    seq_t mod = (seq_t)n * 2 + (seq_t)1;

    /* factorial modulo mod, starting with 1! = 1 */
    seq_t fact = (seq_t)1;
    seq_t sum  = (seq_t)0;

    /* OEIS comment: for n > 1, a(n) < 2*n; for n = 1 the value is 2.
       To be safe we examine all m up to 2*n (inclusive). */
    idx_t max_m = n * 2;
    if (max_m < 1)   /* should not happen because n==0 was handled above */
        return (seq_t)0;

    for (idx_t m = 1; m <= max_m; ++m)
    {
        fact = (fact * (seq_t)m) % mod;
        sum  = (sum + fact) % mod;
        if (sum == (seq_t)0)
            return (seq_t)m;
    }
    return (seq_t)0;
}
