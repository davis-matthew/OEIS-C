/*
 * A096047  a(n) = B(2n,4)/B(2n) (see comment).
 * https://oeis.org/A096047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(4*16^n+4^n-2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=22, a(2)=346, a(n)=21*a(n-1)-84*a(n-2)+64*a(n-3) for n≥3 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;             /* cached values of the sequence */
static idx_t cache_cap = 0;             /* capacity of the cache array   */
static idx_t cache_last = 0;            /* last index for which a value is stored (>=0) */

seq_t a(idx_t n) {
    /* Ensure cache exists and has space up to n */
    if (!cache) {
        cache_cap = 8;                 /* start with a reasonable size */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failure – should not happen */
        /* initialise the first three terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)346;
        cache_last = 2;
    }

    /* Grow the cache if necessary */
    if (n > cache_last) {
        idx_t need = n;
        if (need + 1 > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap <= need + 1) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }
        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_last + 1; i <= need; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)22;
            } else if (i == 2) {
                cache[2] = (seq_t)346;
            } else {
                cache[i] = cache[i-1] * ((seq_t)21)
                           - cache[i-2] * ((seq_t)84)
                           + cache[i-3] * ((seq_t)64);
            }
        }
        cache_last = need;
    }

    return cache[(size_t)n];
}
