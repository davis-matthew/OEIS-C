/*
 * A180192  Triangle read by rows: T(n,k) is the number of permutations of [n]
 *          having k fixed blocks.
 * https://oeis.org/A180192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where (r,k) satisfy offset_row(r)+k=n; T(r,k) =
 *            sum_{j=k}^{r+1-k} binom(j-1,k-1) * binom(r+1-j,k) * d(r-j), d(i)
 *            are derangements (d(0)=1,d(1)=0, d(i)=(i-1)*(d(i-1)+d(i-2)))
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 62, u64 n <= 130,
 *            u128 n <= 340, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Binomial coefficient utility */
#include "../utils/binom.h"

/* Cached derangement numbers */
static seq_t *der_cache = NULL;
static size_t der_cache_size = 0;

/* Ensure der_cache covers 0 .. n */
static void ensure_derangements(idx_t n)
{
    if (der_cache_size > n)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(der_cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    der_cache = tmp;

    if (der_cache_size == 0) {
        der_cache[0] = (seq_t)1;      /* d(0) = 1 */
        if (new_size > 1)
            der_cache[1] = (seq_t)0;  /* d(1) = 0 */
    }

    for (size_t i = der_cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            der_cache[0] = (seq_t)1;
        } else if (i == 1) {
            der_cache[1] = (seq_t)0;
        } else {
            seq_t a = der_cache[i - 1];
            seq_t b = der_cache[i - 2];
            der_cache[i] = (seq_t)(i - 1) * (a + b);
        }
    }
    der_cache_size = new_size;
}

/* offset_row(r): starting index of row r in the flattened triangle */
static idx_t offset_row(idx_t r)
{
    /* r + sum_{i=0}^{r-1} ceil(i/2) */
    if ((r & 1u) == 0) {
        idx_t q = r >> 1;
        return r + (idx_t)((unsigned __int128)q * q);
    } else {
        idx_t q = r >> 1;
        return r + (idx_t)((unsigned __int128)q * (q + 1));
    }
}

/* Main OEIS sequence */
seq_t a(idx_t n)
{
    /* Locate row r and column k such that n = offset_row(r) + k */
    idx_t low = 0, high = 1;

    /* Expand high until offset_row(high) > n */
    while (offset_row(high) <= n) {
        high <<= 1;
        if (high == 0) /* overflow guard */
            break;
    }

    /* Binary search for r */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (offset_row(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;
    idx_t k = n - offset_row(r);

    /* Ensure derangements up to r are available */
    ensure_derangements(r);

    if (k == 0) {
        /* No fixed blocks → derangement count */
        return der_cache[r];
    }

    /* Compute T(r,k) by the defining summation */
    seq_t sum = 0;
    for (idx_t j = k; j <= r + 1 - k; ++j) {
        seq_t t = binom(j - 1, k - 1);
        t = t * binom(r + 1 - j, k);
        t = t * der_cache[r - j];
        sum += t;
    }
    return sum;
}
