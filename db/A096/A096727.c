/*
 * A096727  Expansion of eta(q)^8 / eta(q^2)^4 in powers of q.
 * https://oeis.org/A096727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -8*σ(n) + 48*σ(n/2) - 64*σ(n/4) (σ(m) = sum of divisors;
 *            σ(k) = 0 for non‑integral k). a(0) = 1
 * Range:     i8 n <= 9, i16 n <= 1469, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   sigma_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_helper.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Cached σ values.  We keep two pieces of state:
   - sigma_cap : current allocated capacity of the cache
   - sigma_max : highest index for which σ has been computed
   The cache may be larger than sigma_max, but indices > sigma_max are
   considered uninitialized. */
static seq_t *sigma_cache = NULL;   /* array of σ values */
static idx_t sigma_cap   = 0;       /* allocated size */
static idx_t sigma_max   = 0;       /* highest index computed (>=0) */

/* Grow cache to at least new_len elements (new_len > sigma_cap).
   The old data remains valid. */
static void grow_cache(idx_t new_len)
{
    seq_t *new_cache = (seq_t *)realloc(sigma_cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(1);
    sigma_cache = new_cache;
    sigma_cap   = new_len;
}

/* Ensure σ values are computed up to and including index n. */
static void ensure_sigma_up_to(idx_t n)
{
    if (n <= sigma_max) return;          /* already known */

    /* Ensure capacity. */
    idx_t req = n + 1;                    /* need indices 0..n inclusive */
    if (req > sigma_cap) {
        idx_t new_cap = (sigma_cap == 0) ? 1 : sigma_cap;
        while (new_cap < req) new_cap <<= 1;   /* power of two grow */
        grow_cache(new_cap);
    }

    /* Compute new σ values, starting just after the last known. */
    idx_t start = sigma_max;
    if (sigma_max == 0) start = 0;        /* ensure 0 is computed on first call */
    for (idx_t i = start; i <= n; ++i)
        sigma_cache[i] = sigma_helper(i);

    sigma_max = n;                        /* mark as fully computed up to n */
}

/* --------------------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* Ensure required σ values are ready. */
    ensure_sigma_up_to(n);
    if (n % 2 == 0)   ensure_sigma_up_to(n / 2);
    if (n % 4 == 0)   ensure_sigma_up_to(n / 4);

    seq_t s1 = sigma_cache[n];
    seq_t s2 = (n % 2 == 0) ? sigma_cache[n / 2] : (seq_t)0;
    seq_t s3 = (n % 4 == 0) ? sigma_cache[n / 4] : (seq_t)0;

    /* Compute the formula using seq_t arithmetic. */
    seq_t val = - (seq_t)8 * s1 + (seq_t)48 * s2 - (seq_t)64 * s3;
    return val;
}
