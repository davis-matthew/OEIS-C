/*
 * A169166  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169166: Number of reduced words of length n in the Coxeter group
 * on 9 generators with relations S_i^2 = (S_i S_j)^27 = I.
 *
 * Recurrence:
 *   a(0) = 1
 *   a(n) = 2 + 7 * Σ_{k=1}^{n} a(n-k)          for 1 ≤ n ≤ 26
 *   a(n) = 7 * Σ_{k=1}^{26} a(n-k) – 28*a(n-27) for n ≥ 27
 *
 * The sequence is generated on demand, caching previous values.
 */

#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* sequence cache */
static size_t cache_size = 0;        /* number of computed elements */
static size_t cache_cap   = 0;        /* allocated capacity */

static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_size) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 27;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        seq_free_array(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute terms up to index n, extending the cache and internal sums.
 * Recurrence uses sliding window sums for efficient O(n) time. */
static void compute_terms_up_to(idx_t n)
{
    static seq_t total_sum = 0;  /* sum of all computed terms up to last_index */
    static seq_t sum26    = 0;  /* sum of the last 26 terms (for n ≥ 27) */

    for (size_t k = cache_size; k <= (size_t)n; ++k)
    {
        if (k == 0)
        {
            cache[0] = 1;          /* a(0) = 1 */
            total_sum = 1;
        }
        else if (k <= 26)
        {
            seq_t val = 2 + 7 * total_sum;
            cache[k] = val;
            total_sum += val;
        }
        else if (k == 27)
        {
            /* sum26 = a(1)+…+a(26) = total_sum – a(0) */
            sum26 = total_sum - cache[0];
            seq_t val = 7 * sum26 - 28 * cache[0];
            cache[27] = val;
            total_sum += val;
            /* prepare sum26 for next step: sum of a(2)…a(27) */
            sum26 += cache[27] - cache[1];
        }
        else /* k > 27 */
        {
            seq_t val = 7 * sum26 - 28 * cache[k - 27];
            cache[k] = val;
            total_sum += val;
            /* advance sliding window: drop a(k-26), add a(k) */
            sum26 += cache[k] - cache[k - 26];
        }
    }

    cache_size = (size_t)n + 1;
}

/* Public interface required by the grading system */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    compute_terms_up_to(n);
    return cache[(size_t)n];
}
