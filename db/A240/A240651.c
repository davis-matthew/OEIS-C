/*
 * A240651  Number of nX3 0..1 arrays with no element equal to exactly one
 *          horizontal or vertical neighbor, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 2*a(n-3) * - 11*a(n-4) - 12*a(n-5) -
 *            9*a(n-6) + 15*a(n-7) * + 23*a(n-8) + 20*a(n-9) - 6*a(n-10) -
 *            15*a(n-11) * - 12*a(n-12) - 2*a(n-13) + 3*a(n-14) + a(n-15) * *
 *            Initial values: a(1)=1, a(2)=2, a(3)=9, a(4)=17, a(5)=43, *
 *            a(6)=136, a(7)=402, a(8)=1180, a(9)=3518, * a(10...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>   /* for size_t */

static const seq_t init[15] = {
    (seq_t)1,   (seq_t)2,   (seq_t)9,   (seq_t)17,  (seq_t)43,
    (seq_t)136, (seq_t)402, (seq_t)1180,(seq_t)3518,(seq_t)10525,
    (seq_t)31454,(seq_t)94059,(seq_t)281484,(seq_t)842574,(seq_t)2522415
};

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms actually stored */
static size_t cache_cap  = 0;   /* allocated capacity (>= cache_size) */

static void ensure_cache(size_t n)
{
    if (n < 1) return;          /* nothing to do for offset < 1 */

    if (cache_size >= n) return;       /* already have enough terms */

    /* Allocate or grow the cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 16;                      /* start with a small block */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }
    while (cache_cap < n) cache_cap <<= 1;
    if (cache_cap > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* Initialise the first 15 terms only once */
    if (cache_size == 0) {
        memcpy(cache, init, 15 * sizeof(seq_t));
        cache_size = 15;
    }

    /* Compute new terms using the recurrence */
    for (size_t i = cache_size; i < n; ++i) {
        size_t idx = i;                 /* 0‑based index for a(idx+1) */
        seq_t s = 0;
        s += (seq_t)3  * cache[idx-1];
        s += cache[idx-2];
        s += (seq_t)2  * cache[idx-3];
        s -= (seq_t)11 * cache[idx-4];
        s -= (seq_t)12 * cache[idx-5];
        s -= (seq_t)9  * cache[idx-6];
        s += (seq_t)15 * cache[idx-7];
        s += (seq_t)23 * cache[idx-8];
        s += (seq_t)20 * cache[idx-9];
        s -= (seq_t)6  * cache[idx-10];
        s -= (seq_t)15 * cache[idx-11];
        s -= (seq_t)12 * cache[idx-12];
        s -= (seq_t)2  * cache[idx-13];
        s += (seq_t)3  * cache[idx-14];
        s += cache[idx-15];
        cache[i] = s;
    }

    if (cache_size < n) cache_size = n;   /* update stored size */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for indices < 1 */
    size_t N = (size_t)n;                /* safe conversion from idx_t */
    ensure_cache(N);
    return cache[n-1];                    /* zero‑based indexing */
}
