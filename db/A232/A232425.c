/*
 * A232425  Number of (n+1) X (1+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          vertically.
 * https://oeis.org/A232425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + 11*a(n-3) - a(n-4) - 28*a(n-5) -
 *            32*a(n-6), a(1)=4, a(2)=16, a(3)=64, a(4)=240, a(5)=988,
 *            a(6)=3964
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* unused, sequence starts at n=1 */

    /* initialise cache with first 6 terms if not already done */
    if (cache == NULL) {
        cache_max = 6;
        cache = (seq_t*)malloc((size_t)(cache_max + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 4;
        cache[2] = 16;
        cache[3] = 64;
        cache[4] = 240;
        cache[5] = 988;
        cache[6] = 3964;
    }

    /* grow cache if required */
    if (n > cache_max) {
        size_t newsize = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = cache[i-1];
            val += (seq_t)10 * cache[i-2];
            val += (seq_t)11 * cache[i-3];
            val -= cache[i-4];
            val -= (seq_t)28 * cache[i-5];
            val -= (seq_t)32 * cache[i-6];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
