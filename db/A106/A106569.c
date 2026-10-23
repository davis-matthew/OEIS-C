/*
 * A106569  a(n) = 5*a(n-1) + 3*a(n-2), where a(0) = 0, a(1) = 3.
 * https://oeis.org/A106569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2), a(0) = 0, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t  cap   = 0;      /* size of allocated cache   */
    static idx_t  len   = 0;      /* number of terms stored    */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 2;                                  /* store a(0) and a(1)      */
        cache = (seq_t *)malloc(cap * sizeof(seq_t)); /* cast for C++          */
        if (!cache) return 0;                      /* out‑of‑memory fallback   */
        cache[0] = 0;                              /* a(0)                     */
        cache[1] = 3;                              /* a(1)                     */
        len = 2;
    }

    /* value already known */
    if (n < len)
        return cache[n];

    /* grow cache if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;                       /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                      /* out‑of‑memory fallback   */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms via recurrence */
    for (idx_t i = len; i <= n; ++i)
        cache[i] = 5 * cache[i-1] + 3 * cache[i-2];

    len = n + 1;  /* update number of stored terms */
    return cache[n];
}
