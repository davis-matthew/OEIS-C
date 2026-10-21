/*
 * A099674  Partial sums of repdigits of A002281.
 * https://oeis.org/A099674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 12*a(n‑1) – 21*a(n‑2) + 10*a(n‑3) (n > 3) Initial values:
 *            a(0) = 0, a(1) = 7, a(2) = 84
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cache for memoised values */
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;   /* computed terms */
static size_t cache_cap = 0;  /* capacity allocated */
static size_t cache_len = 0;  /* number of terms stored */

/* Ensure the cache can hold at least `needed` elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A099674 (partial sums of repdigits of 7) */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* idx_t → size_t, safe for test range */

    /* Compute missing terms up to idx if necessary */
    if (idx >= cache_len) {
        ensure_cache_capacity(idx + 1);
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)7;
            } else if (i == 2) {
                cache[2] = (seq_t)84;
            } else {
                /* Use recurrence: a(n) = 12*a(n-1) - 21*a(n-2) + 10*a(n-3) */
                cache[i] = ((seq_t)12) * cache[i-1]
                         - ((seq_t)21) * cache[i-2]
                         + ((seq_t)10) * cache[i-3];
            }
        }
        cache_len = idx + 1;
    }

    return cache[(size_t)n];
}
