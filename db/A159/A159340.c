/*
 * A159340  Transform of the finite sequence (1, 0, -1) by the T_{0,1}
 *          transformation (see link).
 * https://oeis.org/A159340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n >= 5
   Initial values: a(0)=2, a(1)=3, a(2)=6, a(3)=16, a(4)=38 */
#include <stdlib.h>

static seq_t *s_cache = NULL;
static size_t s_cache_len = 0;
static size_t s_cache_cap = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    /* initialize cache with the first five values if empty */
    if (s_cache_len == 0) {
        const size_t init_len = 5;
        if (s_cache_cap < init_len) {
            s_cache_cap = init_len;
            s_cache = (seq_t *)realloc(s_cache, s_cache_cap * sizeof(seq_t));
        }
        s_cache[0] = (seq_t)2;
        s_cache[1] = (seq_t)3;
        s_cache[2] = (seq_t)6;
        s_cache[3] = (seq_t)16;
        s_cache[4] = (seq_t)38;
        s_cache_len = init_len;
    }

    if (idx >= s_cache_len) {
        /* grow cache to at least idx+1 */
        if (s_cache_cap < idx + 1) {
            size_t new_cap = s_cache_cap ? s_cache_cap * 2 : 16;
            if (new_cap < idx + 1) new_cap = idx + 1;
            s_cache_cap = new_cap;
            s_cache = (seq_t *)realloc(s_cache, s_cache_cap * sizeof(seq_t));
        }
        /* compute missing terms using the recurrence */
        for (idx_t i = (idx_t)s_cache_len; i <= n; ++i) {
            seq_t t1 = (seq_t)3 * s_cache[i-1];
            seq_t t2 = (seq_t)2 * s_cache[i-2];
            s_cache[i] = t1 - t2 + s_cache[i-3];
        }
        s_cache_len = idx + 1;
    }
    return s_cache[idx];
}
