/*
 * A218997  Unmatched value maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 n X 3 array.
 * https://oeis.org/A218997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 15*a(n-2) + 66*a(n-3) + 63*a(n-4) + 72*a(n-5);
 *            Initial values: a(1)=4, a(2)=46, a(3)=328, a(4)=2374, a(5)=17776
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0; /* number of terms currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* initialise cache with first 5 terms */
    if (!cache) {
        idx_t init_sz = 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_sz);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)46;
        cache[2] = (seq_t)328;
        cache[3] = (seq_t)2374;
        cache[4] = (seq_t)17776;
        cache_size = init_sz;
    }

    /* grow cache if necessary and compute missing terms */
    if (n > cache_size) {
        idx_t new_sz = n;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_sz);
        for (idx_t i = cache_size; i < n; ++i) {
            if (i < 5) continue; /* terms already known */
            seq_t t0 = cache[i-1];
            seq_t t1 = cache[i-2];
            seq_t t2 = cache[i-3];
            seq_t t3 = cache[i-4];
            seq_t t4 = cache[i-5];
            cache[i] = 4*t0 + 15*t1 + 66*t2 + 63*t3 + 72*t4;
        }
        cache_size = n;
    }

    return cache[n-1];
}
