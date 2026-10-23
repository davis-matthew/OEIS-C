/*
 * A121804  1045*6^n/27-513*2^(n-2)-2072*3^(n-3)+670*(-1)^n*3^(n-3)+254*(-1)^(n+1),
 *          n>1.
 * https://oeis.org/A121804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) +5*a(n-2) -75*a(n-3) +36*a(n-4) +108*a(n-5), for
 *            n>6, with a(1)=0, a(2)=159, a(3)=4846, a(4)=43648, a(5)=272432,
 *            a(6)=1759444
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cache[1] .. cache[cap-1] contain computed terms */
static idx_t cap   = 0;       /* allocated size of cache (in entries) */
static idx_t last  = 0;       /* highest index that has been computed (≥0) */

seq_t a(idx_t n)
{
    /* Return the first six terms directly */
    if (n <= 6) {
        switch (n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)159;
            case 3: return (seq_t)4846;
            case 4: return (seq_t)43648;
            case 5: return (seq_t)272432;
            case 6: return (seq_t)1759444;
            default: return (seq_t)0;   /* unreachable for n>=1 */
        }
    }

    /* Ensure the cache is large enough to hold index n */
    idx_t needed = n + 1;          /* indices 0 .. n (index 0 unused) */
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();        /* memory allocation failed */
        cache = tmp;
        /* Initialise the newly allocated entries */
        for (idx_t i = cap; i < needed; ++i) cache[i] = (seq_t)0;
        /* Store the initial six values (they are the same each time) */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)159;
        cache[3] = (seq_t)4846;
        cache[4] = (seq_t)43648;
        cache[5] = (seq_t)272432;
        cache[6] = (seq_t)1759444;
        /* Update the last computed index if needed */
        if (cap <= 6) last = 6;
        else if (last < 6) last = 6;
        cap = needed;
    }

    /* Compute missing terms up to n, if any */
    idx_t start = (last < 7) ? 7 : last + 1;
    if (start <= n) {
        for (idx_t i = start; i <= n; ++i) {
            seq_t term1 = (seq_t)7   * cache[i - 1];
            seq_t term2 = (seq_t)5   * cache[i - 2];
            seq_t term3 = (seq_t)36  * cache[i - 4];
            seq_t term4 = (seq_t)108 * cache[i - 5];
            seq_t sum   = term1 + term2 + term3 + term4;
            seq_t termm = (seq_t)75  * cache[i - 3];
            cache[i] = sum - termm;
        }
        last = n;
    }

    return cache[n];
}
