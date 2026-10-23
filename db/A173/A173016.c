/*
 * A173016  Numbers k such that the sequence B = B_k defined by {B(1) = 1; for
 *          i >= 2: B(i) = the smallest number h such that sigma(h) =
 *          A000203(h) = B(i-1) + k; or B(i) = 0 if no such number h exists}
 *          is not the sequence {A063524(j): j >= 1}.
 * https://oeis.org/A173016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that k or k+1 equals σ(m) for some
 *            m (σ = sum‑of‑divisors)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Maximum bound for precomputation.  Adjusted to cover ~10000 terms comfortably. */
#define MAX_BOUND 200000

static bool *is_sigma = NULL;          /* flags indicating whether an integer is a σ value       */
static idx_t sigma_max_val = 0;        /* maximum σ value encountered during precomputation     */
static bool sigma_prepared = false;    /* flag indicating that the σ table is set up            */

static seq_t *seq_cache = NULL;        /* computed terms of A173016                               */
static idx_t seq_cache_len = 0;        /* number of terms currently stored                       */
static idx_t seq_cache_cap = 0;        /* capacity of seq_cache                                 */
static idx_t next_k = 1;               /* next integer k to test for inclusion                  */

/* Grow the sequence cache when more space is needed. */
static void seq_grow_cache(idx_t needed)
{
    if (needed > seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cache_cap = (idx_t)new_cap;
    }
}

/* Pre‑compute all σ values up to MAX_BOUND and build a fast lookup table.  */
static void prepare_sigma(void)
{
    if (sigma_prepared) return;

    /* Allocate array to hold σ(n) values.  Use a wider type to avoid overflow. */
    seq_t *sigma_val = (seq_t *)malloc((MAX_BOUND + 1) * sizeof(seq_t));
    if (!sigma_val) abort();

    for (idx_t i = 0; i <= MAX_BOUND; ++i) sigma_val[i] = 0;

    /* Classical sieve for σ(n): add each divisor i to all multiples.  */
    for (idx_t i = 1; i <= MAX_BOUND; ++i) {
        for (idx_t j = i; j <= MAX_BOUND; j += i) {
            sigma_val[j] += (seq_t)i;
        }
    }

    /* Determine maximum σ(n) to size the lookup table.  */
    for (idx_t i = 1; i <= MAX_BOUND; ++i) {
        idx_t v = (idx_t)sigma_val[i];
        if (v > sigma_max_val) sigma_max_val = v;
    }

    /* Build the boolean table.  */
    is_sigma = (bool *)malloc((sigma_max_val + 3) * sizeof(bool));
    if (!is_sigma) abort();
    for (idx_t i = 0; i <= sigma_max_val + 2; ++i) is_sigma[i] = false;

    for (idx_t i = 1; i <= MAX_BOUND; ++i) {
        idx_t v = (idx_t)sigma_val[i];
        if (v <= sigma_max_val + 2) is_sigma[v] = true;
    }

    free(sigma_val);
    sigma_prepared = true;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but return 0 safely */
    prepare_sigma();

    /* Compute terms until we have at least n of them.  */
    while (seq_cache_len < n) {
        seq_t k = next_k;
        bool ok = false;
        /* Check if k or k+1 is a σ value.  */
        if ((idx_t)k <= sigma_max_val && is_sigma[(idx_t)k]) ok = true;
        else if ((idx_t)(k + 1) <= sigma_max_val && is_sigma[(idx_t)(k + 1)]) ok = true;

        if (ok) {
            seq_grow_cache(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = k;
        }
        ++next_k;
    }

    return seq_cache[n - 1];
}
