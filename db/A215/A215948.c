/*
 * A215948  a(n) = 3^n*A(2*n), where A(n) = 3*A(n-1) + A(n-2) - A(n-3)/3 with
 *          A(0)=A(1)=3, A(2)=11.
 * https://oeis.org/A215948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*a(n-1) - 27*a(n-2) + 3*a(n-3); Initial values: a(0) =
 *            3, a(1) = 33, a(2) = 1035
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Compute the n-th term of OEIS A215948 */
seq_t a(idx_t n)
{
    /* Ensure base array is at least three elements long */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)33;
        cache[2] = (seq_t)1035;
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();            /* allocation failure */
        cache = tmp;
        /* Compute new entries using the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)33 * cache[i - 1]
                     - (seq_t)27 * cache[i - 2]
                     + (seq_t)3  * cache[i - 3];
        }
        cache_len = new_len;
    }

    return cache[n];
}
