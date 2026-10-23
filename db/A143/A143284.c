/*
 * A143284  Number of binary words of length n containing at least one subword
 *          100001 and no subwords 10^{i}1 with i<4.
 * https://oeis.org/A143284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - a(n-7) - a(n-11) a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=2, a(8)=3,
 *            a(9)=4, a(10)=5
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 77, u64 n <= 156,
 *            u128 n <= 313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoized table of a(k) for k <= current max */
    static size_t cap   = 0;             /* current capacity of cache (number of entries) */

    /* initial values for 0 <= n <= 10 */
    static const seq_t base[11] = {0,0,0,0,0,0,1,2,3,4,5};

    /* small indices are cheap to look up */
    if (n < 11)
        return base[(size_t)n];

    size_t need = (size_t)n + 1;         /* we must store indices 0 … n */

    if (cap < need) {
        /* grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp)                 /* allocation failure: return 0 */
            return 0;
        cache = tmp;

        /* initialise the new portion with zeros */
        for (size_t i = cap; i < need; ++i)
            cache[i] = 0;

        /* copy base values that may still be missing */
        if (cap <= 11) {
            size_t up = (cap < 11 ? 11 : cap);
            for (size_t i = cap; i < up; ++i)
                cache[i] = base[i];
        }

        size_t oldcap = cap;
        cap = need;

        /* compute terms from max(oldcap,11) up to cap-1 */
        size_t start = oldcap;
        if (start < 11) start = 11;
        for (size_t i = start; i < cap; ++i) {
            cache[i] = cache[i-1] * 2;          /* 2*a(n-1)            */
            cache[i] += cache[i-5];             /* +a(n-5)             */
            cache[i] -= cache[i-2];             /* -a(n-2)             */
            cache[i] -= cache[i-7];             /* -a(n-7)             */
            cache[i] -= cache[i-11];            /* -a(n-11)            */
        }
    }

    return cache[(size_t)n];
}
