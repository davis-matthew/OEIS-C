/*
 * A027780  a(n) = 7*(n+1)*binomial(n+2,7)/2.
 * https://oeis.org/A027780
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,6)*C(n+2,2) (equivalent to 7*(n+1)*C(n+2,7)/2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the binomial coefficient C(n,k) for any non‑negative k <= n.
   The routine uses the multiplicative formula and divides by k at each step
   so that intermediate values stay within the range of the native
   integer type used for seq_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;         /* take the smaller k for fewer steps */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* The arithmetic is performed in the promoted integer type
           (at least 32 bits); the result of each division is exact
           for successive binomial products. */
        res = res * (seq_t)(n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* By definition the sequence starts at n = 5.  If a call is made
       for a smaller index the function returns 0, which is acceptable
       because the problem statement specifies that behaviour below
       the offset does not matter. */
    if (n < 5) return (seq_t)0;

    seq_t part1 = binom(n + 1, 6);   /* C(n+1,6) */
    seq_t part2 = binom(n + 2, 2);   /* C(n+2,2) */
    return part1 * part2;            /* product is exact for all fitted values */
}
