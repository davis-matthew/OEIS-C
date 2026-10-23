/*
 * A196141  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,4,1,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(n) = 3*a(n-1) -2*a(n-2) +a(n-3) -3*a(n-4) +3*a(n-5) 
 *                  -a(n-6) -7*a(n-8) +a(n-9) +4*a(n-10) +2*a(n-11) +6*a(n-12)
 * Initial values: a(1)=4, a(2)=8, a(3)=7, a(4)=26, a(5)=49, a(6)=85,
 *                 a(7)=178, a(8)=348, a(9)=683, a(10)=1349, a(11)=2688, a(12)=5319
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* dynamic array of computed terms */
static idx_t cache_len = 0;       /* one past the largest index stored (so cache_len-1 is max index) */

/* Ensure that cache contains terms up to and including index n */
static void grow_cache(idx_t n)
{
    /* initial allocation and seed with known values */
    if (cache == NULL) {
        cache_len = 13;                 /* indices 0..12 (0 unused) */
        cache    = (seq_t*)calloc(cache_len, sizeof(seq_t));
        if (!cache) return;             /* allocation failed – caller will get 0 */

        /* hard‑coded initial values (index 0 unused) */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)26;
        cache[5] = (seq_t)49;
        cache[6] = (seq_t)85;
        cache[7] = (seq_t)178;
        cache[8] = (seq_t)348;
        cache[9] = (seq_t)683;
        cache[10] = (seq_t)1349;
        cache[11] = (seq_t)2688;
        cache[12] = (seq_t)5319;
    }

    /* grow array if necessary */
    if (n >= cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return;                /* allocation failed – leave cache unchanged */
        cache = tmp;
        /* fill new slots (initialized to 0 by realloc for size growth) */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute a(i) using the recurrence */
            seq_t val = (seq_t)3 * cache[i-1]
                        - (seq_t)2 * cache[i-2]
                        + cache[i-3]
                        - (seq_t)3 * cache[i-4]
                        + (seq_t)3 * cache[i-5]
                        - cache[i-6]
                        - (seq_t)7 * cache[i-8]
                        + cache[i-9]
                        + (seq_t)4 * cache[i-10]
                        + (seq_t)2 * cache[i-11]
                        + (seq_t)6 * cache[i-12];
            cache[i] = val;
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    grow_cache(n);
    return cache[n];
}
