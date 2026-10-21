/*
 * A223506  Petersen graph (3,1) coloring a rectangular array: number of 3 X n
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves along an edge
 *          of this graph, with the array starting at 0.
 * https://oeis.org/A223506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 12*a(n-1) - 4*a(n-2) - 73*a(n-3) +
 *            103*a(n-4) - 23*a(n-5) - 16*a(n-6) + 4*a(n-7) initial values
 *            (a(1) … a(8)): a(1) = 36, a(2) = 121, a(3) = 1519, a(4) = 16323,
 *            a(5) = 182901, a(6) = 2030665, a(7) = 22598167, a(8) = 251348043
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the sequence A223506.  Maintains an internal cache so that any
   sequence value can be obtained in O(1) after an initial O(n) fill. */
seq_t a(idx_t n)
{
    /* ---- static cache ------------------------------------------- */
    static seq_t *cache       = NULL;   /* array of seq_t, indexes 0…cache_sz */
    static idx_t  cache_sz    = 0;      /* highest n computed and stored */
    static size_t cache_cap   = 0;      /* allocated capacity (>= cache_sz+1) */

    /* ---- base initial values ------------------------------------- */
    static const seq_t init[9] = {
        0ULL,               /* dummy for index 0 */
        (seq_t)36ULL,       /* a(1) */
        (seq_t)121ULL,      /* a(2) */
        (seq_t)1519ULL,     /* a(3) */
        (seq_t)16323ULL,    /* a(4) */
        (seq_t)182901ULL,   /* a(5) */
        (seq_t)2030665ULL,  /* a(6) */
        (seq_t)22598167ULL, /* a(7) */
        (seq_t)251348043ULL /* a(8) */
    };

    /* ---- handle requests below the cache size --------------------- */
    if (n <= cache_sz)
        return cache[(size_t)n];

    /* ---- allocating or enlarging the cache ------------------------ */
    if (!cache) {                         /* first allocation */
        cache_cap = (cache_sz > 8 ? cache_sz : 8) + 1; /* allocate at least upto 8 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure */

        /* initialise the first eight entries with the known values  */
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = init[i];
        cache_sz = 8;
    }

    /* grow capacity to include requested n */
    if (n > cache_sz) {
        if (n + 1 > cache_cap) {
            size_t new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute new terms using the empirical recurrence */
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t t5 = cache[i - 5];
            seq_t t6 = cache[i - 6];
            seq_t t7 = cache[i - 7];

            seq_t pos = t1 * (seq_t)12 + t4 * (seq_t)103 + t7 * (seq_t)4;
            seq_t neg = t2 * (seq_t)4 + t3 * (seq_t)73 + t5 * (seq_t)23 + t6 * (seq_t)16;

            cache[i] = pos - neg;
        }

        cache_sz = n;
    }

    return cache[(size_t)n];
}
