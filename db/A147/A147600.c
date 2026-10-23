/*
 * A147600  Expansion of 1/(1 - 3*x^2 + x^4).
 * https://oeis.org/A147600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is odd; a(2k) = F(2(k+1)), k ≥ 0 where F(i) is the
 *            i-th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 45, u64 n <= 91, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Odd indices are zero */
    if ((n & (idx_t)1) != 0) return (seq_t)0;

    /* Index of the required Fibonacci number */
    idx_t m = n + 2;          /* n = 2k → m = 2(k+1) */

    /* Static cache of Fibonacci numbers */
    static seq_t *fib_cache  = NULL;  /* array of F(0)...F(m) */
    static idx_t  fib_len    = 0;     /* number of elements currently stored */
    static idx_t  fib_cap    = 0;     /* allocated capacity */

    /* Allocate initial cache if needed */
    if (fib_cache == NULL) {
        fib_cap = 4;                        /* arbitrary small start */
        fib_cache = (seq_t *)malloc(sizeof(seq_t) * fib_cap);
        if (fib_cache == NULL) return (seq_t)0;  /* allocation failure */
        fib_cache[0] = 0;                    /* F(0) */
        fib_cache[1] = 1;                    /* F(1) */
        fib_len = 2;
    }

    /* Grow the cache if it does not yet contain F(m) */
    if (m >= fib_len) {
        idx_t needed = m + 1;                 /* indices 0..m inclusive */
        if (needed > fib_cap) {
            idx_t new_cap = fib_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(fib_cache,
                                           sizeof(seq_t) * new_cap);
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            fib_cache = tmp;
            fib_cap   = new_cap;
        }
        for (idx_t i = fib_len; i <= m; ++i) {
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
        }
        fib_len = m + 1;
    }

    return fib_cache[m];
}
