/*
 * A183046  Sums of knight's moves from (0,0) to vertical segments (n,-n) to
 *          (n,n) on infinite chessboard.
 * https://oeis.org/A183046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-4) - 2*a(n-5) + a(n-7) for n ≥ 10
 *            Initial values: a(0)=0, a(1)=7, a(2)=12, a(3)=17, a(4)=26,
 *            a(5)=39, a(6)=50, a(7)=71, a(8)=84, a(9)=109
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {0,7,12,17,26,39,50,71,84,109};

static seq_t *cache = NULL;
static size_t  cache_len = 0;   /* number of values stored (>= cache_len) */
static size_t  cache_cap = 0;   /* allocated capacity */ 

static void ensure_cache(size_t need)
{
    if (need < cache_cap) return;           /* already large enough */
    size_t new_cap = (cache_cap > 0) ? cache_cap : 16;
    while (new_cap <= need) new_cap <<= 1;   /* double until big enough */

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);     /* fatal if we cannot allocate */

    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;

    if (cache_len == 0) {
        /* fill initial terms */
        for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_len = 10;
    }
}

seq_t a(idx_t n)
{
    if (n < 10) return init_vals[(size_t)n];

    size_t idx = (size_t)n;

    /* Ensure we have enough storage */
    if (cache_len == 0) ensure_cache(idx);
    else if (idx >= cache_cap) ensure_cache(idx);

    /* Compute any new terms up to idx, if needed */
    if (cache_len <= idx) {
        for (size_t k = cache_len; k <= idx; ++k) {
            seq_t val = cache[k-2] * 2;   /* 2*a(k-2) */
            val += cache[k-3];            /* + a(k-3) */
            val -= cache[k-4];            /* - a(k-4) */
            val -= cache[k-5] * 2;        /* - 2*a(k-5) */
            val += cache[k-7];            /* + a(k-7) */
            cache[k] = val;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
