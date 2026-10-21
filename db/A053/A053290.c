/*
 * A053290  Number of nonsingular n X n matrices over GF(3).
 * https://oeis.org/A053290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (3^n - 3^k) (number of nonsingular n×n
 *            matrices over GF(3))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoized computation */
    static seq_t *cache = NULL;        /* array of computed values a(0)..a(m) */
    static size_t cache_cap = 0;       /* allocated capacity of cache */
    static size_t cache_len = 0;       /* number of values currently stored (last index + 1) */

    /* state of iterative formula for the last computed n */
    static seq_t power3 = 0;              /* 3^k for current k (starting with k=0 => 1) */
    static seq_t pow_factor = 0;          /* 3^(k(k-1)/2) at current k */
    static seq_t prod_qi_minus1 = 0;      /* product_{i=1..k} (3^i - 1) at current k */
    static idx_t current_n = 0;           /* last n that the state corresponds to */

    /* initialization on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;     /* out-of-memory; caller should handle */
        power3 = 1;                      /* 3^0 */
        pow_factor = 1;                  /* 3^{0·-1/2} = 1 */
        prod_qi_minus1 = 1;              /* empty product */
        cache[0] = 1;                    /* a(0) = 1 */
        cache_len = 1;
        current_n = 0;
    }

    /* enlarge cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if the requested n is already computed, return it */
    if (n < cache_len) return cache[n];

    /* compute values incrementally up to n */
    for (idx_t k = current_n + 1; k <= n; ++k) {
        /* previous power3 is 3^(k-1) */
        seq_t prev_power3 = power3;
        /* update power3 to 3^k */
        power3 = prev_power3 * (seq_t)3;

        /* update pow_factor: multiply by 3^(k-1) */
        pow_factor = pow_factor * prev_power3;

        /* update prod_qi_minus1: multiply by (3^k - 1) */
        prod_qi_minus1 = prod_qi_minus1 * (power3 - (seq_t)1);

        /* the current a(k) */
        seq_t val = pow_factor * prod_qi_minus1;

        cache[(size_t)k] = val;
    }

    current_n = n;
    cache_len = n + 1;
    return cache[n];
}
