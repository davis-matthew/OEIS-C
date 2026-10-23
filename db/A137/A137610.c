/*
 * A137610  Self-convolution of A014062, where A014062(n) = C(n^2, n).
 * https://oeis.org/A137610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k^2 , k) * binomial((n-k)^2 , n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the externally supplied binomial helper
 * (see ../utils/binomial.h).  A small cache keeps the values
 * C(k^2,k) for all k up to the maximum n seen so far, allowing each
 * call to run in O(n) time.  The code is fully portable to any of the
 * supported seq_t types. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t n, idx_t k) */

/* Static cache for C(k^2, k). */
static seq_t *binom_cache = NULL;
static size_t binom_len   = 0; /* number of initialized entries */
static size_t binom_cap   = 0; /* current capacity of binom_cache */

/* Allocate or grow the cache to hold at least (new_cap) entries. */
static void ensure_binoms_cap(size_t new_cap)
{
    if (new_cap <= binom_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(binom_cache, new_cap * sizeof(seq_t));
    if (!tmp) {                     /* allocation failed: keep existing cache */
        /* In a real application we might abort, but for safety we
           simply keep the old cache and continue with partial data. */
        return;
    }
    binom_cache = tmp;
    binom_cap   = new_cap;
}

/* Compute and store C(k^2, k) for all k up to n (inclusive). */
static void ensure_binoms_upto(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (target <= binom_len)
        return;

    ensure_binoms_cap(target);

    for (size_t i = binom_len; i < target; ++i) {
        idx_t sq = (idx_t)i * (idx_t)i;
        binom_cache[i] = binomial(sq, (idx_t)i);
    }
    binom_len = target;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains all required binomial values. */
    ensure_binoms_upto(n);

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t left  = binom_cache[(size_t)k];
        idx_t nk    = n - k;
        seq_t right = binom_cache[(size_t)nk];
        sum += left * right;
    }
    return sum;
}
