/*
 * A169063  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * Let a(0)=1. For k≥1 write c_k = 1 if k==24, 2 for 1≤k≤23, else
 *            0. * Then for all n≥0: * a(n) = c_n + 48 * Σ_{i=1}^{min(23,n)}
 *            a(n-i) if n≤23 * a(24)= 1 + 48 * Σ_{i=1}^{23} a(24-i) - 1176 *
 *            a(0) * a(n) = 48 * Σ_{i=1}^{23} a(n-i) - 1176 * a(n-24) if n>24
 *            * * This follows from the generating funct...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cap   = 0;
#endif
    static idx_t computed_max = (idx_t)-1; /* sentinel for uninitialised */

    /* Initialisation: a(0)=1. */
    if (computed_max == (idx_t)-1) {
#ifdef __cplusplus
        cache.resize(1);
        cache[0] = (seq_t)1;
#else
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        cap = 1;
#endif
        computed_max = 0;
    }

    if (n <= computed_max) {
        return cache[n];
    }

    /* Ensure storage for all indices up to n. */
#ifdef __cplusplus
    if (cache.size() <= n) cache.resize(n + 1);
#else
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(EXIT_FAILURE); /* allocation failure */
        cache = new_ptr;
        cap = new_cap;
    }
#endif

    /* Compute successive terms up to n. */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= 23 && k >= i; ++i) {
            sum += cache[k - i];
        }

        seq_t term;
        if (k <= 23) {
            seq_t c = 2;           /* c_k for 1≤k≤23 */
            term = c + (seq_t)48 * sum;
        } else if (k == 24) {
            seq_t c = 1;           /* c_24 */
            term = c + (seq_t)48 * sum - (seq_t)1176 * cache[0];
        } else { /* k > 24 */
            term = (seq_t)48 * sum - (seq_t)1176 * cache[k - 24];
        }

        cache[k] = term;
        computed_max = k;
    }

    return cache[n];
}
