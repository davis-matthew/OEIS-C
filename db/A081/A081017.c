/*
 * A081017  a(n) = Lucas(4n+1) - 1, or 5*Fibonacci(2n)*Fibonacci(2n+1).
 * https://oeis.org/A081017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4n+1) - 1 = 5*Fibonacci(2n)*Fibonacci(2n+1)
 *            Recurrence: a(0)=0, a(1)=10, a(2)=75 a(n) = 8*a(n-1) - 8*a(n-2)
 *            + a(n-3) (n ≥ 3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* two static variables are required: `size` – how many
       values have actually been computed; and `cap` – how many
       slots have been allocated in the cache.  The cache itself
       is a zero‑initialised array. */
    static seq_t *cache = NULL;
    static idx_t size = 0;   /* number of terms stored (next free index) */
    static idx_t cap  = 0;   /* allocated capacity of cache */

    /* initialise the cache with the first three terms */
    if (cache == NULL) {
        cap  = 3;
        size = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)75;
    }

    /* expand the cache if n is beyond what we have already
       generated.  We keep `size` as the number of values that
       are present, and `cap` as the memory size. */
    if (n + 1 > size) {
        /* ensure we have a large enough capacity */
        if (cap < n + 1) {
            idx_t new_cap = cap * 2;
            if (new_cap < n + 1) new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();          /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }
        /* compute any missing terms using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            seq_t term1 = (seq_t)8 * cache[i - 1];
            seq_t term2 = (seq_t)8 * cache[i - 2];
            cache[i] = term1 - term2 + cache[i - 3];
        }
        size = n + 1;
    }

    return cache[n];
}
