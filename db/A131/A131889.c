/*
 * A131889  a(n) is the number of shapes of balanced trees with constant
 *          branching factor 3 and n nodes. The node is balanced if the size,
 *          measured in nodes, of each pair of its children differ by at most
 *          one node.
 * https://oeis.org/A131889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 49, u64 n <= 134,
 *            u128 n <= 161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = a(1) = 1;
 *          a(3n+1+m) = C(3,m) * a(n+1)^m * a(n)^(3-m)   (n ≥ 0, 0 ≤ m ≤ 3)
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed a(i) values, index 0.. */
    static size_t cap = 0;        /* current capacity of cache array */

    /* base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* ensure cache has space for index n */
    if (cache == NULL || cap <= n) {
        size_t newcap = n + 1;   /* at least up to n (inclusive) */
        /* allocate new cache */
        seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newptr)
            return (seq_t)0;     /* out‑of‑memory, return 0 as sentinel */

        /* copy existing data if any */
        if (cache && cap > 0)
            memcpy(newptr, cache, cap * sizeof(seq_t));

        /* clear new part */
        if (newcap > cap)
            memset(newptr + cap, 0, (newcap - cap) * sizeof(seq_t));

        /* initialize known base values if not already present */
        if (newcap > 0)
            newptr[0] = (seq_t)1;
        if (newcap > 1)
            newptr[1] = (seq_t)1;

        /* free old cache */
        if (cache) free(cache);

        cache = newptr;
        cap = newcap;
    }

    /* return cached value if available */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute using recursion */
    idx_t k  = (n - 1) / 3;            /* the “n” in the recurrence */
    int  m   = (int)((n - 1) % 3);     /* 0 ≤ m ≤ 2 for n ≥ 2 */

    /* recursive calls (guaranteed to be smaller indices) */
    seq_t val1 = a(k + 1);             /* a(n+1) in the recurrence */
    seq_t val2 = a(k);                 /* a(n)   in the recurrence */

    seq_t result;

    switch (m) {
        case 0:                     /* C(3,0) * a(k+1)^0 * a(k)^3 */
            result = val2 * val2;   /* val2^2 */
            result = result * val2; /* val2^3 */
            break;
        case 1:                     /* C(3,1) * a(k+1)^1 * a(k)^2 */
            result = (seq_t)3;
            result = result * val1;
            if (val2 != (seq_t)0) {
                seq_t tmp = val2 * val2; /* val2^2 */
                result = result * tmp;
            }
            break;
        case 2:                     /* C(3,2) * a(k+1)^2 * a(k)^1 */
            result = (seq_t)3;
            if (val1 != (seq_t)0) {
                seq_t tmp = val1 * val1; /* val1^2 */
                result = result * tmp;
            }
            result = result * val2;
            break;
        default:                    /* unreachable for n≥2 */
            result = (seq_t)0;
            break;
    }

    cache[n] = result;
    return result;
}
