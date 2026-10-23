/*
 * A192909  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) - a(n-5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=9, a(4)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   ensure_capacity, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of previously computed terms */
    static idx_t max_computed = (idx_t)-1;   /* largest index currently stored */
    static size_t cap = 0;           /* allocated capacity of cache (elements) */

    /* make sure base terms are initialised */
    if (max_computed == (idx_t)-1) {
        /* need space for indices 0..4 */
        size_t need = 5;
        if (need > cap) {
            ensure_capacity(need, &cap, (void **)&cache, sizeof(seq_t));
        }
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 9;
        cache[4] = 27;
        max_computed = 4;
    }

    /* if requested term is already cached, return it immediately */
    if (n <= max_computed) return cache[n];

    /* extend the cache up to n */
    size_t need = (size_t)(n + 1);
    if (need > cap) {
        ensure_capacity(need, &cap, (void **)&cache, sizeof(seq_t));
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Use mul_small to avoid overflow of unsigned integers.
           mul_small(x,m) returns x * m as a seq_t value. */
        seq_t val = mul_small(cache[i-1], 4);              /* 4*a(i-1)   */
        val = val - mul_small(cache[i-2], 3);              /* -3*a(i-2)   */
        val = val + cache[i-3];                            /* +a(i-3)     */
        val = val - cache[i-5];                            /* -a(i-5)     */
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
