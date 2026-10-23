/*
 * A152687  Partial products operator applied thrice to nonzero Fibonacci
 *          numbers.
 * https://oeis.org/A152687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} (∏_{j=1}^{k} (∏_{i=1}^{j} F_i)), where F_1=1,
 *            F_2=1, F_{m+1}=F_m+F_{m-1}. This is the partial products
 *            operator applied thrice to nonzero Fibonacci numbers
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation.  cache[1] == a(1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored (>=0) */
    static size_t cache_cap = 0;   /* capacity of cache array   */

    /* State variables that are kept between calls.            */
    static seq_t p1 = 1;          /* product of Fibonacci numbers up to current index */
    static seq_t p2 = 1;          /* product of the above products up to current index */
    static seq_t p3 = 1;          /* product of the above products up to current index */
    static seq_t fib_prev = 1;    /* F(m)  for the current index m */
    static seq_t fib_curr = 1;    /* F(m+1) for the current index m */

    if (n == 0) /* offset is 1; values for n<1 are undefined but return 0 */
        return (seq_t)0;

    if (n > cache_len) {
        /* Grow cache if necessary.  Allocate at least n+1 elements
           because we use 1‑based indexing. */
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
            while (newcap < needed) newcap *= 2;
            seq_t *newarr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!newarr) abort(); /* memory allocation failure */
            cache = newarr;
            cache_cap = newcap;
        }

        /* Compute values from cache_len+1 up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Current Fibonacci number is fib_prev (F_i). */
            p1 *= fib_prev;    /* P1(i) = P1(i-1) * F_i */
            p2 *= p1;          /* P2(i) = P2(i-1) * P1(i) */
            p3 *= p2;          /* P3(i) = P3(i-1) * P2(i) */
            cache[i] = p3;     /* a(i) = P3(i) */

            /* Advance Fibonacci: F_{i+1} = fib_prev + fib_curr. */
            seq_t tmp = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = tmp;
        }
        cache_len = n;
    }

    return cache[n];
}
