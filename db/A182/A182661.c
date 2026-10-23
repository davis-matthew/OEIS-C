/*
 * A182661  Expansion of x^3*exp(-x)/(3*(1-x)).
 * https://oeis.org/A182661
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * C(n,3) * D(n-3) where D(m) is the number of
 *            derangements of m elements
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Used from: A182661, OEIS: Expansion of x^3*exp(-x)/(3*(1-x)) */

#include <stdlib.h>

/* Memoised derangement numbers D[0..] */
static seq_t *D_cache = NULL;
static idx_t D_cache_len = 0;   /* number of values stored (next to compute is D_cache_len) */
static idx_t D_cache_cap = 0;   /* allocated capacity */

/* Ensure we have D values up to and including m */
static void ensure_D_up_to(idx_t m)
{
    if (m < D_cache_len) return;               /* already computed */
    /* Grow capacity to at least m+1 */
    idx_t new_cap = D_cache_cap ? D_cache_cap : 2;
    while (new_cap <= m) new_cap *= 2;
    D_cache = (seq_t *)realloc(D_cache, new_cap * sizeof(seq_t));
    /* Compute missing values */
    for (idx_t i = D_cache_len; i <= m; ++i) {
        if (i == 0) {
            D_cache[0] = (seq_t)1;
        } else if (i == 1) {
            D_cache[1] = (seq_t)0;
        } else {
            D_cache[i] = ((seq_t)(i - 1)) * (D_cache[i - 1] + D_cache[i - 2]);
        }
    }
    D_cache_len = m + 1;
    D_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Offset starts at n = 3.  For n < 3 the value is undefined, return 0. */
    if (n < 3) return (seq_t)0;

    /* Compute derangements D(n-3) on demand */
    idx_t m = n - 3;
    ensure_D_up_to(m);

    /* Compute binomial C(n,3) = n*(n-1)*(n-2)/6 */
    seq_t comb = (seq_t)n;
    comb *= (seq_t)(n - 1);
    comb *= (seq_t)(n - 2);
    comb /= (seq_t)6;

    /* a(n) = 2 * comb * D[n-3] */
    comb *= (seq_t)2;
    comb *= D_cache[m];

    return comb;
}
