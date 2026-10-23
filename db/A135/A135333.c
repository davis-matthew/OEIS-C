/*
 * A135333  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n having k DUDU's starting at level 1.
 * https://oeis.org/A135333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 172, u64 n <= 631,
 *            u128 n <= 2347, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A135333
    T(n,k) = d(0,k)*Catalan(n-1)
             + Σ_{j=k}^{n-2} (-1)^(j-k)(j+3)C(j,k)C(2n-j-2,n)/(n+1)
    a(n) is the flattened triangle read by rows.
    a(0)=1, a(1)=1, a(2)=2, …
    The function builds the triangle row by row, caching the values.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

static seq_t *cache     = NULL;           /* cached values of a(n) */
static size_t cache_len = 0;              /* number of values stored */
static size_t cache_cap = 0;              /* allocated capacity */
static idx_t  next_row   = 0;              /* next row to compute */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;  /* on OOM we silently abort future work */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the triangle row 'n' and append its entries to the cache. */
static void compute_row(idx_t n)
{
    /* Number of terms to add for this row. */
    idx_t add_terms = (n < 3) ? 1 : (n - 1);
    ensure_cache(cache_len + (size_t)add_terms);

    if (n == 0) {
        cache[cache_len++] = (seq_t)1;                /* T(0,0) */
    } else if (n == 1) {
        cache[cache_len++] = (seq_t)1;                /* T(1,0) */
    } else if (n == 2) {
        cache[cache_len++] = (seq_t)2;                /* T(2,0) */
    } else {
        /* Rows n >= 3 contain n-1 entries, k = 0 .. n-2. */
        for (idx_t k = 0; k < n - 1; ++k) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;

            for (idx_t j = k; j <= n - 2; ++j) {
                seq_t bin_jk = binomial(j, k);
                seq_t bin_jn = binomial(2 * n - j - 2, n);
                seq_t term   = bin_jk * bin_jn;
                term *= (seq_t)(j + 3);

                if ((j - k) & 1)
                    sum_neg += term;
                else
                    sum_pos += term;
            }

            seq_t denom = (seq_t)(n + 1);
            seq_t value = (sum_pos - sum_neg) / denom;

            if (k == 0)
                value += catalan(n - 1);

            cache[cache_len++] = value;
        }
    }
}

/* Public entry point requested by the harness. */
seq_t a(idx_t n)
{
    /* Generate rows until we have at least n+1 cached terms. */
    while (cache_len <= n) {
        compute_row(next_row);
        ++next_row;
    }
    return cache[n];
}
