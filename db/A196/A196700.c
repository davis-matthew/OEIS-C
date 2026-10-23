/*
 * A196700  Number of n X 1 0..4 arrays with each element x equal to the
 *          number of its horizontal and vertical neighbors equal to 3,1,0,4,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) for n > 4 * a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=6 * This is equivalent to a(n) =
 *            tribonacci(n+2)-tribonacci(n-2)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache: cache[i] = a(i)   */
static idx_t cache_size = 0;    /* size of allocated memory (>=1 when cache!=NULL) */
static idx_t max_computed = 0;  /* largest index for which cache[i] is valid */

/* Ensure that cache contains a(1)…a(n). */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed) return;          /* already available          */

    size_t new_size = (size_t)(n + 1);       /* need indices 0 … n        */

    if (new_size > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return;                    /* allocation failure        */
        cache = tmp;
        cache_size = new_size;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) cache[i] = 1;
        else if (i == 2) cache[i] = 2;
        else if (i == 3) cache[i] = 4;
        else if (i == 4) cache[i] = 6;
        else
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* sequence starts at n = 1 */
    ensure_cache(n);
    return cache[n];
}
