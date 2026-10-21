/*
 * A063212  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          44 ).
 * https://oeis.org/A063212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) (with a(1)=1, a(2)=3, a(3)=3,
 *            a(4)=7, a(5)=7)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms cached, cache[0] is a(1) */

    if (n == 0) return (seq_t)0;    /* undefined, return 0 for safety */

    /* Initialize first 5 terms if not already done */
    if (cache_size < 5) {
        size_t needed = 5;
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
        cache[3] = (seq_t)7;   /* a(4) */
        cache[4] = (seq_t)7;   /* a(5) */
        cache_size = 5;
    }

    /* If requested term is already cached */
    if (n <= cache_size) {
        return cache[n-1];
    }

    /* Extend cache to hold up to term n */
    size_t new_size = (size_t)n;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;

    /* Compute missing terms using recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t term = cache[i-3] + cache[i-4] - cache[i-6];
        cache[i-1] = term;   /* store a(i) */
    }

    return cache[n-1];
}
