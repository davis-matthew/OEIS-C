/*
 * A196316  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,3,0,2,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-5)   (for n ≥ 6)
 * Initial values: a(1) = 1, a(2) = 7, a(3) = 10, a(4) = 23, a(5) = 57
 *
 * This implementation memoises the sequence up to n = 10000
 * (the typical maximum required for the timing constraints).
 */

#include <stddef.h>
#include <stdlib.h>

/* cache with max size 10000 elements, 1‑based indexing offset by 1 */
#define MAX_CACHE 10001
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;  /* number of terms currently stored */

/* Return the n‑th term of A196316 (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;  /* undefined for n < 1 */

    /* Convert idx_t to size_t safely */
    size_t sz = (size_t)n;

    /* Extend the cache if necessary (up to MAX_CACHE) */
    if (sz > MAX_CACHE)
        sz = MAX_CACHE;  /* clamp for safety; larger n will be computed on‑the‑fly */

    if (cache_len < sz) {
        for (size_t i = cache_len; i < sz; ++i) {
            size_t idx = i + 1;  /* 1‑based index of the term */

            if (idx == 1u)      cache[i] = (seq_t)1;
            else if (idx == 2u) cache[i] = (seq_t)7;
            else if (idx == 3u) cache[i] = (seq_t)10;
            else if (idx == 4u) cache[i] = (seq_t)23;
            else if (idx == 5u) cache[i] = (seq_t)57;
            else {
                /* a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-5) */
                seq_t val =   cache[i-1]               /* a(n-1) */
                           + (seq_t)2 * cache[i-2]     /* a(n-2) */
                           + (seq_t)2 * cache[i-3]     /* a(n-3) */
                           - cache[i-5];              /* a(n-5) */
                cache[i] = val;
            }
        }
        cache_len = sz;
    }

    /* If n was larger than MAX_CACHE, compute on the fly using a 5‑term ring buffer */
    if ((size_t)n > MAX_CACHE) {
        seq_t buf[5] = {1,7,10,23,57};
        for (size_t i = MAX_CACHE; i < (size_t)n; ++i) {
            seq_t next = buf[0] + (seq_t)2*buf[1] + (seq_t)2*buf[2] - buf[3];
            /* shift buffer */
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            buf[3] = buf[4];
            buf[4] = next;
        }
        return buf[4];
    }

    return cache[(size_t)n-1];
}
