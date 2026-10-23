/*
 * A197988  Number of isomorphism classes of nanocones with 3 pentagons and a
 *          symmetric boundary of length n.
 * https://oeis.org/A197988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A197988: Number of isomorphism classes of nanocones with 3 pentagons
   and a symmetric boundary of length n.
   Recurrence (order 8) derived from the generating function:
     a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) + 2*a(n-6) + a(n-7) - a(n-8)
   (for n > 8; the first eight terms are given explicitly). */
#include <stdlib.h>

static const seq_t init[9] = {
    0,                       /* a(0) unused */
    (seq_t)1,                /* a(1) */
    (seq_t)3,                /* a(2) */
    (seq_t)5,                /* a(3) */
    (seq_t)12,               /* a(4) */
    (seq_t)18,               /* a(5) */
    (seq_t)31,               /* a(6) */
    (seq_t)44,               /* a(7) */
    (seq_t)67                /* a(8) */
};

static seq_t *cache = NULL;   /* memoized values a(1)..a(max_n) */
static idx_t cache_len = 0;   /* largest n stored (>=1) */

/* Compute a(n) for n>=1.  Results are cached;  any order of calls is fine. */
seq_t a(idx_t n)
{
    if (n <= 8)
        return init[n];

    /* Ensure cache has space up to n */
    if (!cache) {
        cache = (seq_t*)calloc(9, sizeof(seq_t));      /* indices 0..8, 0 unused */
        if (!cache) return 0;                         /* allocation failure */
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }
    if (n > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;                     /* allocation failure */
        cache = new_cache;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];
            seq_t a8 = cache[i-8];

            /* Positive part: a1 + 2*a2 + 2*a6 + a7 */
            seq_t pos = a1 + (a2 << 1) + (a6 << 1) + a7;
            /* Negative part: a3 + 2*a4 + a5 + a8 */
            seq_t neg = a3 + (a4 << 1) + a5 + a8;

            cache[i] = pos - neg;           /* pos >= neg for all n>=9 */
        }
        cache_len = n;
    }
    return cache[n];
}
