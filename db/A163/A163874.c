/*
 * A163874  a(n) = n-a(a(n-3)) with a(0) = a(1) = a(2) = 0.
 * https://oeis.org/A163874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(a(n-3)), a(0) = a(1) = a(2) = 0
 * Range:     u8 n <= 411, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t  *cache = NULL;          /* computed values   */
    static unsigned char *found = NULL;   /* 0=not computed    */
    static size_t cap = 0;                /* current capacity  */

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 32;
        while (newcap <= (size_t)n)
            newcap <<= 1;

        seq_t *c = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        unsigned char *f = (unsigned char *)realloc(found, newcap * sizeof(unsigned char));
        if (!c || !f)
            abort();                       /* memory allocation failure */

        cache = c;
        found = f;
        memset(found + cap, 0, (newcap - cap) * sizeof(unsigned char));
        cap = newcap;
    }

    /* return cached value if available */
    if (found[(size_t)n])
        return cache[(size_t)n];

    /* compute value */
    seq_t val;
    if (n < 3) {
        val = 0;
    } else {
        seq_t t1 = a(n - 3);
        /* t1 fits in idx_t because 0 <= a(n) <= n */
        idx_t t1_idx = (idx_t)t1;
        seq_t t2 = a(t1_idx);
        val = (seq_t)n - t2;
    }

    /* store result and mark as computed */
    cache[(size_t)n] = val;
    found[(size_t)n] = 1;
    return val;
}
