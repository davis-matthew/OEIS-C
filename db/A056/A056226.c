/*
 * A056226  Image of Moebius function (A008683) under "little Hankel"
 *          transform that sends [c_0, c_1, ...] to [d_0, d_1, ...] where d_n
 *          = c_n^2 - c_{n+1}*c_{n-1}.
 * https://oeis.org/A056226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μ(n)^2 – μ(n‑1)·μ(n+1) where μ is the Möbius function
 * Range:     i8 n <= 8749, i16 n <= 8749, i32 n <= 8749, i64 n <= 8749,
 *            i128 n <= 8749, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a table of smallest prime factors (spf) for fast μ evaluation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Static cache of small prime factors */
static idx_t *spf_cache = NULL;   /* spf_cache[i] = smallest prime factor of i (spf_cache[1] = 1) */
static size_t spf_cap = 0;        /* number of elements stored in spf_cache (size = spf_cap + 1) */

/* Ensure spf_cache has entries up to 'limit' (inclusive).  Rebuilds the table if needed. */
static void ensure_spf(idx_t limit)
{
    if (limit >= spf_cap) {
        /* free old table if it exists */
        free(spf_cache);
        build_spf(limit, &spf_cache);
        spf_cap = limit + 1;
        /* guarantee that spf_cache[1] = 1 so that μ(1) = 1 with the logic below */
        spf_cache[1] = 1;
    }
}

/* Compute the Möbius function μ(m) using the spf table.  Returns -1, 0, or 1. */
static seq_t mobius(idx_t m)
{
    if (m == 0) return 0;   /* undefined, but safe guard */
    if (m == 1) return 1;

    /* Ensure the spf table covers m */
    if ((idx_t)spf_cap <= m)
        ensure_spf(m);

    seq_t sign = 1;
    idx_t t = m;
    while (t > 1) {
        idx_t p = spf_cache[t];
        unsigned int cnt = 0;
        while (t % p == 0) {
            t /= p;
            cnt++;
            if (cnt > 1)   /* squared prime factor found */
                return 0;
        }
        sign = -sign;     /* each distinct prime flips the sign */
    }
    return sign;
}

/* External function required by the grader. */
seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1; behavior for n < 1 is irrelevant */
        return 0;

    if (n == 1)
        return 0; /* given explicitly in OEIS */

    /* We need μ(n-1), μ(n), μ(n+1).  Ensure spf table covers n+1. */
    if ((idx_t)spf_cap <= n + 1)
        ensure_spf(n + 1);

    seq_t mu_n     = mobius(n);
    seq_t mu_nm1   = mobius(n - 1);
    seq_t mu_np1   = mobius(n + 1);

    /* a(n) = μ(n)^2 – μ(n-1)·μ(n+1) */
    /* All μ values are in {-1,0,1}, so the intermediates fit safely in seq_t. */
    return mu_n * mu_n - mu_nm1 * mu_np1;
}
