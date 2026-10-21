/*
 * A235278  Number of (n+1) X (8+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 21, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235278: Number of (n+1)×9 arrays with a 2×2 diagonal sum
 * property.
 *
 * Formula: a(n) = a(n-1) +15*a(n-2) -15*a(n-3) -80*a(n-4)
 *                +80*a(n-5) +180*a(n-6) -180*a(n-7)
 *                -144*a(n-8) +144*a(n-9)
 *
 * Initial values (offset 1):
 *   a(1)=15520, a(2)=19060, a(3)=23000, a(4)=31504, a(5)=41704,
 *   a(6)=64168, a(7)=93992, a(8)=160888, a(9)=260680.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached sequence values.  1‑based indexing; seq_cache[0] unused. */
static seq_t *seq_cache = NULL;
static idx_t  cached_n  = 0;   /* highest index cached */
static size_t cache_cap = 0;   /* allocated capacity (>= cached_n+1) */

/* Initial values for 1 ≤ n ≤ 9 */
static const seq_t init_vals[10] = {
    0ULL,      /* unused; offset is 1 */
    15520ULL,
    19060ULL,
    23000ULL,
    31504ULL,
    41704ULL,
    64168ULL,
    93992ULL,
    160888ULL,
    260680ULL
};

/* Coefficients of the recurrence: a(n‑1)…a(n‑9) */
static const int16_t coeff[9] = {
    1,    /* a(n-1)   */
    15,   /* a(n-2)   */
   -15,   /* a(n-3)   */
   -80,   /* a(n-4)   */
    80,   /* a(n-5)   */
   180,   /* a(n-6)   */
  -180,   /* a(n-7)   */
  -144,   /* a(n-8)   */
   144    /* a(n-9)   */
};

/* Ensure the cache is large enough for indices up to n inclusive. */
static void ensure_cache(idx_t n) {
    if (seq_cache == NULL) {
        /* Create initial cache covering indices 0..9 (capacity 16). */
        cache_cap = 16;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) return;  /* memory failure – caller guards */

        for (idx_t i = 0; i < cache_cap; ++i)
            seq_cache[i] = 0;   /* default zero */
        for (idx_t i = 1; i <= 9; ++i)
            seq_cache[i] = init_vals[i];
        cached_n = 9;
    }

    if (n <= cached_n) return;  /* already cached */

    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;  /* memory failure – caller guards */
        seq_cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i)
            seq_cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute and store terms up to n. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 9; ++k) {
            seq_t term = seq_cache[i - (idx_t)(k + 1)];
            if (coeff[k] >= 0) {
                pos += (seq_t)(coeff[k]) * term;
            } else {
                neg += (seq_t)(-coeff[k]) * term;
            }
        }
        seq_cache[i] = pos - neg;   /* final result is guaranteed non‑negative */
    }
    cached_n = n;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1; safe fallback */
    ensure_cache(n);
    return seq_cache[n];
}
