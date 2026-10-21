/*
 * A059174  Maximal number of regions into which 5-space can be divided by n
 *          hyperspheres.
 * https://oeis.org/A059174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n-1,5) + Σ_{i=0..5} binomial(n,i) (offset 0)
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 193, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial(n,k) for k ≤ 5 using a 128‑bit intermediate to avoid overflow
   when seq_t is a 64‑bit unsigned type.  For arbitrary‑precision seq_t the cast
   is harmless. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)            /* binomial is zero when k > n */
        return (seq_t)0;
    if (k > n - k)        /* use symmetry to minimise loop iterations */
        k = n - k;

    unsigned __int128 res = 1;
    for (idx_t i = 0; i < k; ++i) {
        /* multiply by (n - k + i + 1) then divide by (i + 1); division is exact */
        res = res * (n - k + i + 1) / (i + 1);
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* a(0) = 1 by definition */
        return (seq_t)1;

    /* term binomial(n‑1,5) – for n ≤ 5 this is zero */
    seq_t result = binom_small(n - 1, 5);

    /* add Σ‑term */
    for (idx_t i = 0; i <= 5; ++i)
        result += binom_small(n, i);

    return result;
}
