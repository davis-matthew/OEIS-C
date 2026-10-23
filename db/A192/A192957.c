/*
 * A192957  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+4) + 4*Fibonacci(n+2) - (n^2+4*n+7)
 *            Recurrence: a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) -
 *            a(n-5), a(0)=0, a(1)=1, a(2)=1, a(3)=5, a(4)=14
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    const seq_t base[5] = {0, 1, 1, 5, 14};
    if (n <= 4) return base[n];

    /* Lazy cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;
    static idx_t last_computed = 4;

    /* Initial allocation */
    if (cache == NULL) {
        cached_len = 64;                /* initial arbitrary size > 5 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cached_len);
        cache[0] = 0; cache[1] = 1; cache[2] = 1;
        cache[3] = 5; cache[4] = 14;
    }

    /* Grow cache if necessary */
    while (cached_len <= n) {
        size_t new_len = cached_len * 2;
        if (new_len == 0) new_len = 1; /* safety for zero */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        cached_len = new_len;
    }

    /* Compute new entries using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i-1]
                 - 5 * cache[i-2]
                 +   cache[i-3]
                 + 2 * cache[i-4]
                 -   cache[i-5];
    }
    last_computed = n;
    return cache[n];
}
