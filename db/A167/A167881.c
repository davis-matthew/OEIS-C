/*
 * A167881  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=3*2^(n-1) for 1≤n≤15,
 *            a(16)=∑_{i=1}^{15}a(i)−a(0)+1, a(n)=∑_{j=1}^{15}a(n−j)−a(n−16)
 *            for n≥17
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   free_seq_array, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/free_seq_array.h"

/* allocate / free helpers ----------------------------------------- */
static seq_t *allocate_seq_array(size_t n) {
    return (seq_t *)malloc(sizeof(seq_t) * n);
}

/* cache for computed values --------------------------------------- */
static seq_t *cache = NULL;     /* values 0 .. cache_len-1 */
static idx_t  cache_len = 0;    /* number of stored values */

/* initialise first 17 terms (0…16) -------------------------------- */
static void init_first_seventeen(void) {
    if (cache != NULL)
        return;
    cache = allocate_seq_array(17);
    cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= 15; ++i) {
        /* a(i)=3*2^(i-1) */
        cache[i] = (seq_t)3 * pow2(i - 1);
    }
    /* a(16) = sum_{i=1}^{15} a(i) - a(0) + 1 */
    seq_t sum = 0;
    for (idx_t i = 1; i <= 15; ++i)
        sum += cache[i];
    cache[16] = sum - cache[0] + (seq_t)1;
    cache_len = 17;
}

/* ensure that values up to n are computed and cached --------------- */
static void ensure_values_up_to(idx_t n) {
    init_first_seventeen();
    if (n < cache_len)
        return;

    /* grow the array to hold indices up to n (inclusive) */
    seq_t *new_cache = allocate_seq_array(n + 1);
    memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
    free_seq_array(cache);
    cache = new_cache;

    /* compute missing terms */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 15; ++j)
            sum += cache[k - j];
        cache[k] = sum - cache[k - 16];
    }
    cache_len = n + 1;
}

/* public function -------------------------------------------------- */
seq_t a(idx_t n) {
    ensure_values_up_to(n);
    return cache[n];
}
