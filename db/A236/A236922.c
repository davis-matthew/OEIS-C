/*
 * A236922  Number of integer solutions to a^2 + b^2 + 4*c^2 + 4*d^2 = n.
 * https://oeis.org/A236922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 8084, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236922: a(n) = number of integer solutions to
 *   a^2 + b^2 + 4*c^2 + 4*d^2 = n
 *
 * This is the convolution r₂ * r₂4 where r₂(k) counts representations
 * of k as a sum of two squares, and r₂4(k) = r₂(k/4) if 4 | k, else 0.
 * Thus
 *   a(n) = Σ_{v=0}^{⌊n/4⌋} r₂(v) * r₂(n-4v)
 *
 * The function below builds r₂ lazily on demand.  It is safe for
 * arbitrary‑precision seq_t and for 64‑bit builds.  All operations
 * use only integer arithmetic as required.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

/* Cached array of r₂ values */
static seq_t *r2_cache = NULL;
static idx_t r2_cache_limit = 0;  /* number of elements cached (len) */

/* Recompute r₂ up to 'limit' (inclusive) and store in r2_cache */
static void recompute_r2(idx_t limit)
{
    /* allocate new array */
    size_t len = (size_t)(limit + 1);
    seq_t *tmp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!tmp) return; /* allocation failed; caller will receive 0 */

    /* compute all a^2 + b^2 up to 'limit' */
    int M = (int)_floor_isqrt_u64(limit);   /* <= sqrt(limit) */
    for (int a = -M; a <= M; ++a) {
        int a2 = a * a;
        for (int b = -M; b <= M; ++b) {
            int sum = a2 + b * b;
            if (sum <= (int)limit) {
                tmp[sum] += 1;  /* we count (a,b) with order and signs */
            }
        }
    }

    /* replace old cache */
    free(r2_cache);
    r2_cache = tmp;
    r2_cache_limit = limit + 1; /* one past the last valid index */
}

/* Ensure that r₂ values are cached up to 'n' */
static void ensure_r2(idx_t n)
{
    if (r2_cache == NULL || r2_cache_limit - 1 < n) {
        recompute_r2(n);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_r2(n);

    seq_t result = 0;
    idx_t max_v = n / 4;
    for (idx_t v = 0; v <= max_v; ++v) {
        result += r2_cache[v] * r2_cache[n - 4 * v];
    }
    return result;
}
