/*
 * A197085  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,3,2,0,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1)+3*a(n-2)+10*a(n-3)-3*a(n-4)+2*a(n-5)+11*a(n-6)
   -15*a(n-7)-15*a(n-8)-2*a(n-9)+a(n-10)-2*a(n-11)+4*a(n-12)+2*a(n-13)
   for n > 13.
   Initial values (n=1..13):
   1, 5, 25, 96, 422, 1780, 7537, 31911, 135283,
   573054, 2427879, 10286742, 43582082. */
#include <stdlib.h>

static const seq_t init_vals[13] = {
    (seq_t)1, (seq_t)5, (seq_t)25, (seq_t)96, (seq_t)422,
    (seq_t)1780, (seq_t)7537, (seq_t)31911, (seq_t)135283,
    (seq_t)573054, (seq_t)2427879, (seq_t)10286742, (seq_t)43582082
};

static seq_t *cache = NULL;   /* cache[0] = a(1), cache[i] = a(i+1) */
static idx_t cache_len = 0;   /* number of terms stored */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, this case is never used */
        return (seq_t)0;

    /* initial initialization of cache */
    if (cache == NULL) {
        cache_len = 13;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();            /* allocation failed */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
    }

    /* extend cache up to n if necessary */
    if ((size_t)n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();                /* reallocation failed */
        cache = tmp;

        for (idx_t i = cache_len; i < n; ++i) {
            /* compute a(i+1) using the recurrence and previous 13 terms */
            seq_t pos = 3 * cache[i-1] + 3 * cache[i-2] + 10 * cache[i-3]
                        + 2 * cache[i-5] + 11 * cache[i-6]
                        + cache[i-10] + 4 * cache[i-12] + 2 * cache[i-13];
            seq_t neg = 3 * cache[i-4] + 15 * cache[i-7] + 15 * cache[i-8]
                        + 2 * cache[i-9] + 2 * cache[i-11];
            cache[i] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
