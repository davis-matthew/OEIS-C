/*
 * A190840  a(n+1) = 4*a(n)*(a(n)+1) for a(0) = 1.
 * https://oeis.org/A190840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = 4*a(n)*(a(n)+1) for a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed sequence values */
    static seq_t *cache = NULL;  /* array of a(i) */
    static idx_t len   = 0;      /* number of elements initialized   */
    static idx_t cap   = 0;      /* current capacity of the array    */

    /* Ensure capacity for indices up to n */
    if (cap <= n) {
        idx_t new_cap = cap ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;

        /* realloc may lose the object state; use reinterpret_cast in C++ */
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize first term a(0) if not yet done */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Return cached value if already computed */
    if (n < len) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t tmp  = prev + (seq_t)1;   /* prev + 1 */
        tmp       = prev * tmp;        /* prev * (prev + 1) */
        tmp       = (seq_t)4 * tmp;    /* 4 * prev * (prev+1) */
        cache[i] = tmp;
    }
    len = n + 1;
    return cache[n];
}
