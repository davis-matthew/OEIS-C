/*
 * A127194  A 10th-order Fibonacci sequence.
 * https://oeis.org/A127194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+…+a(n-10), a(1)…a(10)=1
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 39, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache for all computed terms */
static seq_t *cache = NULL;
static size_t cap = 0;        /* allocated size of the cache */
static uint64_t terms = 0;    /* number of terms currently stored (>=0) */

/* grow the cache to hold at least n terms */
static void ensure_capacity(uint64_t n)
{
#ifdef __cplusplus
    if (cap < n) {
        seq_t *new_cache = new seq_t[n];
        if (cache != NULL) {
            for (size_t i = 0; i < terms; ++i)
                new_cache[i] = cache[i];
            delete [] cache;
        }
        cache = new_cache;
        cap = n;
    }
#else
    if (cap < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort the program (no partial results) */
            abort();
        }
        cache = new_cache;
        cap = n;
    }
#endif
}

/* compute the n-th term (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, return 0 */

    uint64_t index = (uint64_t)n;
    if (index > terms) {            /* need to compute more terms */
        ensure_capacity(index);
        for (uint64_t i = terms + 1; i <= index; ++i) {
            if (i <= 10) {
                cache[i - 1] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (int j = 1; j <= 10; ++j)
                    sum += cache[i - j - 1];
                cache[i - 1] = sum;
            }
        }
        terms = index;
    }
    return cache[index - 1];
}
