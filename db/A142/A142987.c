/*
 * A142987  a(1) = 1, a(2) = 10, a(n+2) = 10*a(n+1) + (n + 1)*(n + 2)*a(n).
 * https://oeis.org/A142987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + (n-1)*n*a(n-2) with a(1)=1, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, return 0 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t len   = 0;        /* number of terms stored (a(1) at index 0) */
    static size_t cap   = 0;        /* allocated capacity in terms */

    size_t need = (size_t)n;        /* terms required */

    if (need <= len)                /* already computed */
        return cache[need - 1];

    /* grow the allocation */
    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap < need) new_cap *= 2;
        void *ptr = realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();          /* allocation failed */
        cache = (seq_t *)ptr;
        cap   = new_cap;
    }

    /* compute new terms */
    for (size_t i = len; i < need; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;     /* a(1) */
        } else if (i == 1) {
            cache[i] = (seq_t)10;    /* a(2) */
        } else {
            /* recurrence: a(k) = 10*a(k-1) + (k-1)*k*a(k-2)
               where i = k-1  ->  k = i+1  */
            seq_t term = cache[i - 1] * (seq_t)10;
            term += cache[i - 2] * (seq_t)i * (seq_t)(i + 1);
            cache[i] = term;
        }
    }

    len = need;                     /* update stored size */
    return cache[need - 1];
}
