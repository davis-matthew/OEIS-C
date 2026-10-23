/*
 * A190913  Sequence A190914 evaluated at the negative index -n.
 * https://oeis.org/A190913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 3*a(n-3) - a(n-5) initial values: a(0)=5,
 *            a(1)=0, a(2)=2, a(3)=9, a(4)=2
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = (idx_t)-1;      /* index of last computed value */

    size_t needed = (size_t)n + 1;

    /* initialise storage on first call */
    if (!cache) {
        size_t init_cap = 8;
        if (init_cap < needed) init_cap = needed;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cap = init_cap;

        /* base terms */
        cache[0] = 5;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 9;
        cache[4] = 2;
        last = 4;
    }
    /* grow buffer if necessary */
    else if (n >= cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= last) return cache[n];

    /* compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + (cache[i - 3] * 3) - cache[i - 5];
    }
    last = n;
    return cache[n];
}
