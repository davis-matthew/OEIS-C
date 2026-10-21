/*
 * A052820  Expansion of e.g.f. 1/(1 - x + log(1 - x)).
 * https://oeis.org/A052820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * a(n-1) + Σ_{k=0}^{n-1} binomial(n,k) * (n-k-1)! *
 *            a(k) a(0) = 1 – This implementation uses a dynamic cache for
 *            a(n) and factorials, calculating binomial coefficients
 *            multiplicatively to avoid any missing external function
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External types supplied by the build. */
extern seq_t binom_product(idx_t n, idx_t k); /* unused – present for reference */

/* The sequence function */
seq_t a(idx_t n)
{
    /* Static cache for a(n) and factorials up to the current maximum n. */
    static seq_t *a_cache   = NULL;
    static seq_t *fact      = NULL;
    static idx_t cache_size = 0;   /* number of entries currently cached */

    /* First call – initialise caches for n = 0. */
    if (!a_cache) {
        cache_size = 1;
        a_cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        fact    = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!a_cache || !fact) return (seq_t)0;   /* out‑of‑memory abort */
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        fact[0]    = (seq_t)1;   /* 0! = 1 */
    }

    /* Extend the cache if requested index is beyond the current range. */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = cache_size * 2;
        if (new_size <= n + 1) new_size = n + 1;

        /* Reallocate storage for a_cache and fact arrays. */
        seq_t *tmp_a = (seq_t *)realloc(a_cache, (size_t)new_size * sizeof(seq_t));
        seq_t *tmp_f = (seq_t *)realloc(fact,    (size_t)new_size * sizeof(seq_t));
        if (!tmp_a || !tmp_f) return (seq_t)0;   /* out‑of‑memory abort */
        a_cache = tmp_a;
        fact    = tmp_f;

        /* Compute new factorials. */
        for (idx_t i = old_size; i < new_size; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* Compute new a(i) values. */
        for (idx_t i = old_size; i < new_size; ++i) {
            if (i == 0) {                /* a(0) already set to 1 */
                a_cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;       /* C(i,0) = 1 */

            /* Sum over k = 0 .. i-1 */
            for (idx_t k = 0; k < i; ++k) {
                if (k > 0) {
                    /* Update binom = C(i,k) from C(i,k-1) */
                    binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
                }

                /* term = C(i,k) * (i-k-1)! * a(k) */
                seq_t term = binom;
                term *= fact[i - k - 1];   /* (i-k-1)! */
                term *= a_cache[k];        /* a(k) */

                sum += term;
            }

            seq_t val = (seq_t)i * a_cache[i - 1] + sum;
            a_cache[i] = val;
        }

        cache_size = new_size;
    }

    return a_cache[n];
}
