/*
 * A223689  Petersen graph (8,2) coloring a rectangular array: number of nX5
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal or antidiagonal ne...
 * https://oeis.org/A223689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by a(1)=1296, a(2)=66816, a(3)=3788640,
 *            a(4)=223096320, a(5)=13402129824, a(6)=814399853760,
 *            a(7)=49817845241568, a(8)=3059068970173824, and for n>8 a(n) =
 *            144*a(n-1) - 7582*a(n-2) + 191344*a(n-3) - 2550861*a(n-4) +
 *            18205352*a(n-5) - 64961020*a(n-6) + 89885952*a(n-7)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range indices. */
    if (n == 0) return (seq_t)0;

    /* Static cache: cache[i] stores a(i+1). */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated slots */
    static size_t sz    = 0;   /* number of terms stored (>= sz >= n when ready) */

    /* Initialise the first eight terms on first call. */
    if (sz == 0) {
        size_t init_sz = 8;
        if (cap < init_sz) {
            seq_t *tmp = (seq_t *)malloc(init_sz * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = init_sz;
        }
        static const seq_t init[8] = {
            (seq_t)1296ULL,
            (seq_t)66816ULL,
            (seq_t)3788640ULL,
            (seq_t)223096320ULL,
            (seq_t)13402129824ULL,
            (seq_t)814399853760ULL,
            (seq_t)49817845241568ULL,
            (seq_t)3059068970173824ULL
        };
        for (size_t i = 0; i < init_sz; ++i)
            cache[i] = init[i];
        sz = init_sz;
    }

    /* Grow cache if we need more room. */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        if (new_cap < 8) new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n inclusive. */
    while (sz < (size_t)n) {
        /* recurrence uses the last seven stored terms */
        seq_t t1 = cache[sz-1]; /* a(n-1) */
        seq_t t2 = cache[sz-2]; /* a(n-2) */
        seq_t t3 = cache[sz-3]; /* a(n-3) */
        seq_t t4 = cache[sz-4]; /* a(n-4) */
        seq_t t5 = cache[sz-5]; /* a(n-5) */
        seq_t t6 = cache[sz-6]; /* a(n-6) */
        seq_t t7 = cache[sz-7]; /* a(n-7) */

        seq_t next =  (seq_t)144ULL * t1
                   - (seq_t)7582ULL * t2
                   + (seq_t)191344ULL * t3
                   - (seq_t)2550861ULL * t4
                   + (seq_t)18205352ULL * t5
                   - (seq_t)64961020ULL * t6
                   + (seq_t)89885952ULL * t7;

        cache[sz] = next;
        ++sz;
    }

    return cache[(size_t)n - 1];
}
