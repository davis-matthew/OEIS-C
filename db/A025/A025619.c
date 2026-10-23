/*
 * A025619  Numbers of form 4^i*7^j, with i, j >= 0.
 * https://oeis.org/A025619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth smallest number of the form 4^i * 7^j (i,j>=0)
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 104, u64 n <= 391,
 *            u128 n <= 1513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a two-pointer merge technique to generate the sequence
 * on demand, storing computed terms in a static cache.  It works for any
 * seq_t that supports the C integer operators used in the code. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static size_t idx4 = 0;
static size_t idx7 = 0;

/* helper: ensure cache has space for one more element */
static void ensure_cache_space(void) {
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* no more memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1; value for n=0 is unused */

    /* initialise cache with first term 1 */
    if (cache_len == 0) {
        ensure_cache_space();
        cache[0] = (seq_t)1;
        cache_len = 1;
        idx4 = 0;
        idx7 = 0;
    }

    /* generate terms until we reach index n */
    while (cache_len < n) {
        seq_t cand4 = cache[idx4] * (seq_t)4;
        seq_t cand7 = cache[idx7] * (seq_t)7;

        seq_t next;
        if (cand4 < cand7) {
            next = cand4;
            idx4++;
        } else if (cand7 < cand4) {
            next = cand7;
            idx7++;
        } else {                /* equal, only one copy of the duplicate */
            next = cand4;
            idx4++;
            idx7++;
        }

        ensure_cache_space();
        cache[cache_len++] = next;
    }

    return cache[n - 1];
}
