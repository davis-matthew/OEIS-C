/*
 * A202125  Number of -n..n arrays of 4 elements with first, second and third
 *          differences also in -n..n.
 * https://oeis.org/A202125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 6*a(n-4) + 6*a(n-5) -
 *            3*a(n-6) + 3*a(n-7) - 3*a(n-8) + a(n-9) Initial values: a(1)=19,
 *            a(2)=127, a(3)=475, a(4)=1279, a(5)=2833, a(6)=5509, a(7)=9739,
 *            a(8)=16039, a(9)=25003
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 193, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset starts at 1 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;         /* largest n that has been computed */

    /* Initial values for the first nine terms */
    static const seq_t init_vals[10] = {
        (seq_t)0,
        (seq_t)19,
        (seq_t)127,
        (seq_t)475,
        (seq_t)1279,
        (seq_t)2833,
        (seq_t)5509,
        (seq_t)9739,
        (seq_t)16039,
        (seq_t)25003
    };

    /* Return immediately if already known */
    if (cache && n <= cache_size) return cache[n];

    /* Allocate or grow the cache to hold indices 0..n (index 0 unused) */
    size_t new_size = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) return (seq_t)0;          /* allocation failure – return 0 */

    /* Place the base values (overlap is harmless) */
    for (idx_t i = 1; i <= 9 && i <= n; ++i)
        cache[i] = init_vals[i];

    /* If we are requesting one of the first nine terms, we are done */
    if (n <= 9) {
        cache_size = n;
        return cache[n];
    }

    /* Compute any missing terms up to n */
    idx_t start = (cache_size > 9) ? cache_size : 9;  /* highest computed index so far */
    for (idx_t i = start + 1; i <= n; ++i) {
        seq_t val = cache[i-1] * 3;
        val -= cache[i-2] * 3;
        val += cache[i-3] * 3;
        val -= cache[i-4] * 6;
        val += cache[i-5] * 6;
        val -= cache[i-6] * 3;
        val += cache[i-7] * 3;
        val -= cache[i-8] * 3;
        val += cache[i-9];
        cache[i] = val;
    }

    cache_size = n;
    return cache[n];
}
