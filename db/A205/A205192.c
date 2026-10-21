/*
 * A205192  Number of (n+1) X 8 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-8) for n > 13, with the first 13 terms as:
 *            a(1)=432, a(2)=440, a(3)=708, a(4)=864, a(5)=1568, a(6)=2048,
 *            a(7)=3904, a(8)=4608, a(9)=7872, a(10)=9216, a(11)=15808,
 *            a(12)=18944, a(13)=32960
 * Range:     u8 none, u16 n <= 14, u32 n <= 44, u64 n <= 104, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for memory functions and abort. */
#include <stdlib.h>
#include <string.h>

/* The function itself: external linkage, following the required signature. */
seq_t a(idx_t n)
{
    /* The first 13 terms (indexing from 1).  Element 0 is unused. */
    static const seq_t init[14] = {
        0,          /* dummy 0‑th entry */
        432, 440, 708, 864, 1568, 2048, 3904,
        4608, 7872, 9216, 15808, 18944, 32960
    };

    /* Cache for memoisation: 1‑based indexing. */
    static seq_t *cache = NULL;   /* points to an array of seq_t */
    static idx_t  cap   = 0;      /* allocated capacity (1‑based) */
    static idx_t  next  = 14;     /* next index that needs to be computed
                                     (i.e. first index > largest computed) */

    /* Handle the first 13 terms directly. */
    if (n <= 13) return init[n];

    /* Allocate or grow the cache if necessary. */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 16 : cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        /* Zero the newly allocated part – safe for all used seq_t types. */
        memset(cache + cap, 0, sizeof(seq_t) * (new_cap - cap));
        /* Initialise the first 13 entries if this is the first allocation. */
        if (cap == 0) {
            for (idx_t i = 1; i <= 13; ++i)
                cache[i] = init[i];
        }
        cap = new_cap;
    }

    /* Compute missing terms up to n.  We start from 'next', guaranteed >=14. */
    for (idx_t i = next; i <= n; ++i) {
        seq_t t = cache[i - 2];   /* cache[i-2] */
        t += t;                   /* 2*a(i-2) */
        t += cache[i - 8];        /* +a(i-8) */
        cache[i] = t;
    }
    next = n + 1;  /* update the next index that remains to be computed */

    return cache[n];
}
