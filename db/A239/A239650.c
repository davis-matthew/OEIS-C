/*
 * A239650  Number of 2 X n 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+12*a(n-2)-18*a(n-3)-29*a(n-4)+19*a(n-5)+38*a(n-6)+8*a(n-7)
 *            with initial terms a(1)=4, a(2)=20, a(3)=92, a(4)=418,
 *            a(5)=1898, a(6)=8588, a(7)=38888 Offset is 1 (a(1) is the first
 *            term)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0;            /* offset starts at 1 */

    if (n <= cache_size) return cache[n - 1];

    /* allocate or grow cache to hold up to a(n) */
    idx_t new_size = n;
    if (!cache) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!p) return 0;            /* allocation failed, unlikely */
        cache = p;
    }
    cache_size = new_size;

    /* initialise the first seven terms (if they are within the requested size) */
    if (new_size >= 1) cache[0] = (seq_t)4;
    if (new_size >= 2) cache[1] = (seq_t)20;
    if (new_size >= 3) cache[2] = (seq_t)92;
    if (new_size >= 4) cache[3] = (seq_t)418;
    if (new_size >= 5) cache[4] = (seq_t)1898;
    if (new_size >= 6) cache[5] = (seq_t)8588;
    if (new_size >= 7) cache[6] = (seq_t)38888;

    /* compute any missing terms using the linear recurrence */
    if (new_size > 7) {
        for (idx_t k = 7; k < new_size; ++k) {
            seq_t t = 0;
            t += (seq_t)3  * cache[k-1];   /* a(k-1) */
            t += (seq_t)12 * cache[k-2];   /* a(k-2) */
            t -= (seq_t)18 * cache[k-3];   /* a(k-3) */
            t -= (seq_t)29 * cache[k-4];   /* a(k-4) */
            t += (seq_t)19 * cache[k-5];   /* a(k-5) */
            t += (seq_t)38 * cache[k-6];   /* a(k-6) */
            t += (seq_t)8  * cache[k-7];   /* a(k-7) */
            cache[k] = t;
        }
    }

    return cache[n - 1];
}
