/*
 * A033758  Expansion of Product_{d | 46} theta_3(q^d).
 * https://oeis.org/A033758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|46} θ₃(q^d)
 * Range:     u8 n <= 575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* θ₃(q) = Σ_{k∈ℤ} q^{k²} */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Internal cache */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Extend the cache to hold values up to up (inclusive). */
static void extend_cache(idx_t up)
{
    if (cache_len > up) return;            /* already up to date */

    /* Allocate new array of size up+1.  Zero-initialise. */
    seq_t *new_cache = (seq_t *)malloc((up + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    for (idx_t i = 0; i <= up; ++i) new_cache[i] = 0;

    /* Compute the coefficients by brute‑force enumeration of all
       quadruples (k₁,k₂,k₃,k₄) such that
           k₁² + 2k₂² + 23k₃² + 46k₄² = n ,
       weighting each term by 2¹ for every non‑zero k.    */
    for (idx_t k1 = 0; k1 * k1 <= up; ++k1) {
        idx_t e1 = k1 * k1;
        seq_t c1 = (e1 == 0) ? 1 : 2;
        idx_t rem1 = up - e1;
        for (idx_t k2 = 0; 2 * k2 * k2 <= rem1; ++k2) {
            idx_t e2 = 2 * k2 * k2;
            seq_t c2 = (e2 == 0) ? 1 : 2;
            idx_t rem2 = rem1 - e2;
            for (idx_t k3 = 0; 23 * k3 * k3 <= rem2; ++k3) {
                idx_t e3 = 23 * k3 * k3;
                seq_t c3 = (e3 == 0) ? 1 : 2;
                idx_t rem3 = rem2 - e3;
                for (idx_t k4 = 0; 46 * k4 * k4 <= rem3; ++k4) {
                    idx_t sum = e1 + e2 + e3 + 46 * k4 * k4;
                    seq_t c4 = (k4 == 0) ? 1 : 2;
                    seq_t weight = c1 * c2;
                    weight = weight * c3;
                    weight = weight * c4;
                    new_cache[sum] += weight;
                }
            }
        }
    }

    /* Replace the old cache.  We keep the old array alive until
       we are sure the new one was allocated successfully. */
    seq_t *old = cache;
    cache = new_cache;
    cache_len = up + 1;
    free(old);
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
