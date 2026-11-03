/*
 * A075701  a(1)=1, a(n+1)=sigma(a(n))-2*a(n).
 * https://oeis.org/A075701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1, a(n+1) = σ(a(n)) – 2*a(n) (σ = sum of divisors)
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-03)
 * Helpers:   isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* ---------- compute sigma (sum of divisors) for n >= 1 ---------- */
static seq_t _sigma_of(idx_t n)
{
    seq_t sum = 0;
    idx_t root = isqrt(n);
    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t q = n / d;
            if (q != d)
                sum += (seq_t)q;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* static cache: a[1] .. a[max_computed] */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    if (n == 0)   /* n is guaranteed ≥1 by the build, but guard anyway */
        return 0;

    /* Grow cache if necessary */
    if (n > max_computed) {
        idx_t new_size = n + 1;   /* use 1‑based indexing */
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache)   /* allocation failed; abort */
            abort();
        cache = new_cache;

        /* Initialize the sequence up to n */
        if (max_computed == 0) {
            cache[1] = (seq_t)1;   /* a(1) = 1 */
            max_computed = 1;
        }

        for (idx_t i = max_computed + 1; i <= n; i++) {
            seq_t prev = cache[i - 1];
            /* Compute σ(|prev|).  If prev == 0, define σ(0) = 0. */
            idx_t uprev = (prev >= 0 ? (idx_t)prev : (idx_t)(-prev));
            seq_t sigma_val = (uprev == 0) ? (seq_t)0 : _sigma_of(uprev);
            cache[i] = sigma_val - (seq_t)2 * prev;
        }
        max_computed = n;
    }

    return cache[n];
}
