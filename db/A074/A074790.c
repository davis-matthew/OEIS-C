/*
 * A074790  a(n) = (2*n+1)!*Sum_{k=0..n} (-1)^k/(2*k+1)!.
 * https://oeis.org/A074790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(2*n+1)*a(n-1) + (-1)^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache   = NULL; /* cache[0] = a(0) */
    static size_t  size   = 0;    /* number of terms stored */
    static size_t  capacity = 0;  /* allocated slots in cache */

    if (cache == NULL) {
        /* first call: allocate space for the initial term a(0) = 1 */
        capacity = 1;
        size     = 1;
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory: return 0 */
        cache[0] = (seq_t)1;
    }

    if (n < (idx_t)size)
        return cache[n];

    /* Grow the cache until it can hold the term a(n). */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        /* Ensure capacity */
        if (size == capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity == 0) new_capacity = 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;  /* out of memory */
            cache = tmp;
            capacity = new_capacity;
        }

        /* Recurrence: a(i) = 2*i*(2*i+1)*a(i-1) + (-1)^i */
        seq_t prev = cache[i - 1];

        seq_t coeff = (seq_t)(2 * i);            /* 2*i  as seq_t */
        coeff *= (seq_t)(2 * i + 1);             /* 2*i*(2*i+1) as seq_t */

        seq_t val = coeff * prev;                /* multiply with previous term */

        if ((i & 1U) != 0)   /* i odd: (-1)^i = -1 */
            val -= (seq_t)1;
        else
            val += (seq_t)1;                           /* i even: (+1) */

        cache[i] = val;
        ++size;
    }

    return cache[n];
}
