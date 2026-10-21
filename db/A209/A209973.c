/*
 * A209973  Number of 2 X 2 matrices having all elements in {0,1,...,n} and
 *          determinant 2.
 * https://oeis.org/A209973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*Σ_{k=1}^{n} φ(k) + 8*Σ_{k=1}^{⌊n/2⌋} φ(k) – 4 (for n≥1;
 *            a(0)=0)
 * Range:     u8 n <= 11, u16 n <= 189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of cumulative sums of Euler's totient function φ.  */
static seq_t *phi_sum_cache = NULL;   /* phi_sum_cache[i] = Σ_{k=1}^{i} φ(k) */
static idx_t   phi_precomputed = 0;   /* largest n for which the cache is valid */

/* Build/extend the cache up to at least n (inclusive). */
static void ensure_phi_sum(idx_t n)
{
    if (n < phi_precomputed)
        return;                       /* already available */

    /* Reallocate cache to the new size.  realloc() frees the old
       block automatically, so we must not free it again. */
    seq_t *new_cache = (seq_t *)realloc(phi_sum_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)      /* memory allocation failure – keep old cache */
        return;

    phi_sum_cache = new_cache;         /* point to the (possibly new) cache */

    /* Allocate temporary array for φ values.  */
    seq_t *phi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!phi) {                         /* allocation failed – clean up */
        /* We cannot free phi_sum_cache here because it is still valid. */
        return;
    }

    /* Initialise φ(i) = i.  */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = (seq_t)i;
    if (n >= 1)
        phi[1] = (seq_t)1;

    /* Compute φ via linear sieve (Euler's algorithm).  */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == (seq_t)i) {           /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / (seq_t)i;
        }
    }

    /* Build cumulative sums for the new part of the cache.  */
    if (phi_precomputed == 0)
        phi_sum_cache[0] = (seq_t)0;
    for (idx_t i = phi_precomputed + 1; i <= n; ++i)
        phi_sum_cache[i] = phi_sum_cache[i - 1] + phi[i];

    free(phi);
    phi_precomputed = n;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* a(0) = 0 */

    ensure_phi_sum(n);

    seq_t sum_n  = phi_sum_cache[n];
    seq_t sum_h  = phi_sum_cache[n / 2];

    return (seq_t)4 * sum_n + (seq_t)8 * sum_h - (seq_t)4;
}
