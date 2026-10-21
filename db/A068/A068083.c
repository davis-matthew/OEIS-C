/*
 * A068083  a(1) = 1, a(n) is the smallest Fibonacci number of the form
 *          k*a(n-1) + 1 with k>0.
 * https://oeis.org/A068083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A068083:
   a(1) = 1;
   a(n) is the smallest Fibonacci number of the form k*a(n−1)+1 with k>0.
   The function constructs the sequence iteratively, memoising results so that
   any call order is supported. */

#include <stddef.h>
#include <stdlib.h>

/* Forward declaration of the helper that finds the next term */
static seq_t compute_next(seq_t prev);

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored (>=1 for a(1)) */
static size_t cache_cap  = 0;   /* capacity of the cache array */

/* Ensure the cache can hold at least `new_cap` elements.
   The function is implemented in a portable way for C and C++.
*/
static void ensure_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t c = cache_cap;
    if (c == 0) c = 4;
    while (c < new_cap) c *= 2;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[c];
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    delete [] cache;
#else
    seq_t *new_cache = (seq_t *)realloc(cache, c * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_cap = c;
}

/* Return the next term given the previous term */
static seq_t compute_next(seq_t prev)
{
    if (prev == (seq_t)1)
        return (seq_t)2;               /* smallest Fibonacci ≥ 2 */

    seq_t target = prev + (seq_t)1;   /* we need fib ≥ prev+1 */
    seq_t fib_prev = (seq_t)0;
    seq_t fib_curr = (seq_t)1;       /* F1 = 1 */

    while (1) {
        if (fib_curr >= target) {
            if ((fib_curr % prev) == (seq_t)1)
                return fib_curr;
        }
        /* next Fibonacci number */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
}

/* Public function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;             /* undefined for n < 1 */

    size_t idx = (size_t)(n - (idx_t)1);

    /* Initialise the cache on the first call */
    if (cache_size == 0) {
        ensure_cap(4);
        cache[0] = (seq_t)1;         /* a(1) */
        cache_size = 1;
    }

    if (idx < cache_size)
        return cache[idx];

    /* Compute missing terms up to n */
    ensure_cap(idx + 1);
    for (size_t i = cache_size; i <= idx; ++i) {
        cache[i] = compute_next(cache[i-1]);
    }
    cache_size = idx + 1;
    return cache[idx];
}
