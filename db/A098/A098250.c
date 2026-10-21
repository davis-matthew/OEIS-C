/*
 * A098250  First differences of Chebyshev polynomials S(n,291) = A098248(n)
 *          with Diophantine property.
 * https://oeis.org/A098250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=291*a(n-1)-a(n-2), a(0)=1, a(1)=290
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for values already computed               */
    static seq_t *cache = NULL;
    static idx_t size = 0;      /* number of elements stored   */
    static idx_t capacity = 0;  /* allocated space (in elems)  */

    /* initialise cache if first call */
    if (!cache) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;           /* out‑of‑memory: sentinel       */
        cache[0] = (seq_t)1;            /* a(0) */
        cache[1] = (seq_t)290;          /* a(1) */
        size = 2;
    }

    /* grow cache until it holds up to a(n) */
    while (size <= n) {
        /* compute next term using the recurrence              */
        seq_t next = cache[size - 1] * (seq_t)291 - cache[size - 2];

        /* resize storage if full                              */
        if (size == capacity) {
            idx_t new_cap = capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* out‑of‑memory: sentinel       */
            cache = tmp;
            capacity = new_cap;
        }

        cache[size++] = next;
    }

    return cache[n];
}
