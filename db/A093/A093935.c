/*
 * A093935  a(1) = 1, a(n+1) = a(n) + n*(a(1) + a(2) + ... + a(n)).
 * https://oeis.org/A093935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n+1)=a(n)+n*(a(1)+...+a(n)) (n>=1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A093935.
   The function uses memoised incremental computation,
   storing each computed term in a static cache. */
seq_t a(idx_t n)
{
    /* static cache of computed terms and running sum */
    static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
    static idx_t cache_n = 0;     /* number of terms currently cached (cache_n >= 0) */
    static seq_t sum = 0;         /* sum_{k=1}^i a(k) for i = cache_n */

    if (n == 0) {                 /* offset is 1; n==0 is not defined */
        return (seq_t)0;
    }

    /* If requested term already cached, return it */
    if (n <= cache_n) {
        return cache[n - 1];
    }

    /* Extend cache to contain up to term n.  Use realloc which works for
       all supported seq_t types (built‑in and class types like cpp_int). */
    cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (cache == NULL) {
        /* allocation failure – in a real program we would handle this error */
        abort();
    }

    /* If this is the first call, initialise a(1) and sum */
    if (cache_n == 0) {
        cache[0] = (seq_t)1;      /* a(1) = 1 */
        sum = (seq_t)1;            /* sum = a(1) */
        cache_n = 1;
    }

    /* Compute new terms sequentially using the recurrence:
           a(k) = a(k-1) + (k-1) * sum_{i=1}^{k-1} a(i)
        and update the running sum. */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        seq_t prev = cache[k - 2];                    /* a(k-1)   */
        seq_t mult = (seq_t)(k - 1) * sum;            /* (k-1)*S   */
        seq_t current = prev + mult;                  /* a(k)      */
        cache[k - 1] = current;                       /* store a(k) */
        sum += current;                               /* update sum */
    }

    cache_n = n;
    return cache[n - 1];
}
