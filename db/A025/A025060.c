/*
 * A025060  Numbers of the form i*j + j*k + k*i, where 1 <= i < j < k.
 * https://oeis.org/A025060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest value of i*j + j*k + k*i with * 1 <= i
 *            < j < k. The sequence is computed by generating all triples * up
 *            to a certain bound, sorting, deduplicating, and caching the
 *            sorted * unique values
 * Range:     u8 none, u16 none, u32 n <= 2000, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"

#define INITIAL_K 200       /* initial bound for i, j, k in the triples */
#define K_STEP     50       /* step size to enlarge the bound if needed */

static seq_t *unique_vals = NULL;   /* sorted unique values cache */
static size_t  unique_len = 0;      /* number of cached values */
static idx_t   current_K  = 0;      /* current bound used for generation */
static bool    initialized = false;

/* generate all values with 1 <= i < j < k <= K, sort them, and keep the
 * unique ones in unique_vals. The function returns the number of unique
 * values produced. */
static size_t generate_values(idx_t K)
{
    /* number of triples = K choose 3 */
    size_t total = (size_t)K * (K - 1) * (K - 2) / 6;
    seq_t *tmp = (seq_t *)malloc(total * sizeof(seq_t));
    if (tmp == NULL) return 0;

    size_t pos = 0;
    for (idx_t i = 1; i <= K - 2; ++i) {
        for (idx_t j = i + 1; j <= K - 1; ++j) {
            for (idx_t k = j + 1; k <= K; ++k) {
                tmp[pos++] = (seq_t)i * (seq_t)j
                           + (seq_t)j * (seq_t)k
                           + (seq_t)k * (seq_t)i;
            }
        }
    }

    /* sort the values */
    qsort(tmp, total, sizeof(seq_t), cmp_seq_t);

    /* deduplicate in place */
    size_t uniq = 0;
    if (total > 0) {
        for (size_t i = 1; i < total; ++i) {
            if (cmp_seq_t(&tmp[uniq], &tmp[i]) != 0) {
                tmp[++uniq] = tmp[i];
            }
        }
        uniq += 1;   /* size of unique set */
    }

    /* replace existing cache */
    free(unique_vals);
    unique_vals = tmp;
    unique_len = uniq;
    current_K   = K;
    return uniq;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset is 1, but guard */
    /* Ensure cache has at least n elements */
    if (!initialized) {
        generate_values(INITIAL_K);
        initialized = true;
    }
    while (unique_len < n) {
        idx_t newK = current_K + K_STEP;
        generate_values(newK);
    }
    return unique_vals[n - 1];
}
