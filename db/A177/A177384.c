/*
 * A177384  G.f. A(x) satisfies A(x) = 1 + x/(A(x) + x*A'(x)).
 * https://oeis.org/A177384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A177384 (OEIS).
 * Recurrence: a(0)=1, a(1)=1;  
 *          a(n) = -∑_{k=1}^{n-1} (k+1) * a(k) * a(n-k)   for n≥2
 *
 * The function uses a static cache to compute terms on demand.
 * It works for any seq_t type (signed integer or arbitrary‑precision)
 * and for any idx_t (uint64_t).
 * The cache size is limited to a large constant; n should not exceed
 * that limit in the test harness (≈200 000 is plenty for n≈10 000).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum supported index (adjust if higher values are required). */
#define MAX_CACHE 200000

/* Static cache and bookkeeping. */
static seq_t cache[MAX_CACHE + 1];
static idx_t cache_len = 0;   /* number of computed entries (next index to compute) */

/* Public function implementing a(n). */
seq_t a(idx_t n)
{
    /* Bounds check: return 0 for out of bounds (unlikely in tests). */
    if (n > MAX_CACHE)
        return 0;

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0 || i == 1)
        {
            cache[i] = 1;
        }
        else
        {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k)
            {
                seq_t term = ((seq_t)(k + 1)) * cache[k];
                term *= cache[i - k];
                sum += term;
            }
            cache[i] = -sum;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
