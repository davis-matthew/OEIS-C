/*
 * A016280  Expansion of 1/((1-2x)(1-3x)(1-11x)).
 * https://oeis.org/A016280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 0,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016280: a(n) = 16*a(n-1) - 61*a(n-2) + 66*a(n-3),
   a(0)=1, a(1)=16, a(2)=195. Only the recurrence is used. */
#include <stddef.h>

static seq_t cache[10001];   /* static array large enough for n ≤ 10000   */
static size_t cache_len = 0; /* number of terms currently computed */

/* Compute terms up to and including idx (0‑based).  idx < 10001. */
static void ensure_cache(size_t idx)
{
    if (idx < cache_len) return;      /* already computed */

    /* Initialise first three terms if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)195;
        cache_len = 3;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len; i <= idx; ++i) {
        cache[i] = (seq_t)16 * cache[i-1]
                 - (seq_t)61 * cache[i-2]
                 + (seq_t)66 * cache[i-3];
    }
    cache_len = idx + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;            /* idx ≤ 10000 for the intended tests   */
    ensure_cache(idx);
    return cache[idx];
}
