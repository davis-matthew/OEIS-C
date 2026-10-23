/*
 * A034764  Dirichlet convolution of sigma(n) with Catalan numbers.
 * https://oeis.org/A034764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} σ(d) * Catalan(n/d - 1) (Catalan(0)=1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

static seq_t *sigma_cache = NULL;   /* divisor sum table, 1‑based */
static idx_t   sigma_cap   = 0;      /* current capacity of sigma_cache */

/* Compute the divisor sums σ(1)…σ(n) and store them in sigma_cache. */
static void compute_sigma_upto(idx_t n)
{
    if (n <= sigma_cap) return;           /* already computed */

    /* allocate a fresh table of size n+1 (index 0 unused) */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return;                     /* allocation failed – silently ignore */

    for (idx_t i = 0; i <= n; ++i) arr[i] = 0;   /* clear the array */

    /* classic sieve for divisor sums */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t vi = (seq_t)i;              /* cast first to avoid overflow */
        for (idx_t j = i; j <= n; j += i) {
            arr[j] += vi;
        }
    }

    /* replace the old table and free the old memory */
    free(sigma_cache);
    sigma_cache = arr;
    sigma_cap   = n;
}

/* Dirichlet convolution of σ and Catalan numbers (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety; offset is 1 */

    compute_sigma_upto(n);                /* ensure σ values up to n exist */

    seq_t sum = 0;

    /* iterate over divisor pairs (i, n/i) with i ≤ √n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor of the pair */
            idx_t d1 = i;
            seq_t sigma_d1 = sigma_cache[d1];
            idx_t k1 = n / d1 - 1;                /* n/d1 - 1 */
            seq_t catalan_k1 = catalan(k1);
            sum += sigma_d1 * catalan_k1;

            /* second divisor of the pair, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t sigma_d2 = sigma_cache[d2];
                /* k2 = n/d2 - 1 = i - 1 */
                seq_t catalan_k2 = catalan(i - 1);
                sum += sigma_d2 * catalan_k2;
            }
        }
    }

    return sum;
}
