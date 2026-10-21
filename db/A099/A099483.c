/*
 * A099483  A Fibonacci convolution.
 * https://oeis.org/A099483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=3*a(n-1)-2*a(n-2)+3*a(n-3)-a(n-4); initial values: a(0)=0,
 *            a(1)=1, a(2)=3, a(3)=7 (recurrence derived from G.f.:
 *            x/((1+x^2)(1-3x+x^2))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached values */
    static seq_t *cache = NULL;
    static size_t computed = 0;   /* number of terms already computed */
    static size_t allocated = 0;   /* current allocation size   */

    /* initialise cache for the first four terms */
    if (computed == 0) {
        allocated = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * allocated);
        if (!cache) return (seq_t)0;           /* unlikely, but safe     */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        computed = 4;
    }

    /* return if we already have the requested value */
    if ((size_t)n < computed)
        return cache[(size_t)n];

    /* ensure sufficient allocation */
    size_t need = (size_t)n + 1;
    if (allocated < need) {
        size_t new_cap = allocated;
        while (new_cap < need)
            new_cap = new_cap + new_cap / 2 + 1;   /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;                 /* allocation failure   */
        cache = tmp;
        allocated = new_cap;
    }

    /* compute missing terms */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t t = 3ULL * cache[i-1] + 3ULL * cache[i-3];
        t -= 2ULL * cache[i-2];
        t -= cache[i-4];
        cache[i] = t;
    }
    computed = (size_t)n + 1;   /* we now have up to a(n) */

    return cache[(size_t)n];
}
