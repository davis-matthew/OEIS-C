/*
 * A120367  a(1) = 1. a(n) = a(n-1) + (maximum number of 1's occurring in the
 *          binary representation of any of the sequence's earlier terms).
 * https://oeis.org/A120367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n)=a(n-1)+max{popcount(a(k)) for 1<=k<n}
 * Range:     u8 n <= 53, u16 n <= 5003, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned popcount_seq(seq_t x) {
    unsigned cnt = 0;
    while (x != 0) {
        cnt += (unsigned)(x & 1);
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n<1, return 0 for safety */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static unsigned max_popcnt = 0;

    size_t req = (size_t)n;
    if (cache == NULL) {                  /* first call: initialise */
        size_t initcap = req > 16 ? req : 16;
        cache = (seq_t *)malloc(initcap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed */
        cache[0] = (seq_t)1;               /* a(1) */
        len = 1;
        max_popcnt = popcount_seq(cache[0]);
        cap = initcap;
    }

    while (len < req) {                   /* extend cache up to n */
        seq_t prev = cache[len-1];
        seq_t next = prev + (seq_t)max_popcnt;

        if (len >= cap) {                 /* grow storage */
            size_t newcap = cap ? cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        cache[len] = next;
        unsigned nxtpop = popcount_seq(next);
        if (nxtpop > max_popcnt) max_popcnt = nxtpop;
        len++;
    }

    return cache[req-1];
}
