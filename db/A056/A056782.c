/*
 * A056782  Number of 3-element proper antichains (i.e., antichains such that
 *          every two members have nonempty intersection) on an unlabeled
 *          n-element set.
 * https://oeis.org/A056782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 100, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence from the rational generating function of OEIS A056782:
 *
 *   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 2*a(n-4) + 4*a(n-5)
 *          + 3*a(n-6) -12*a(n-7) + 3*a(n-8) + 4*a(n-9) + 2*a(n-10)
 *          - 2*a(n-11) - 4*a(n-12) + 4*a(n-13) - a(n-14)          (n >= 14)
 *
 * Initial terms:
 *   a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=18, a(6)=53,
 *   a(7)=135, a(8)=305, a(9)=633, a(10)=1220, a(11)=2217,
 *   a(12)=3834, a(13)=6359
 *
 * The sequence grows rapidly; this implementation keeps a cache to
 * avoid recomputing previously obtained values.  All arithmetic is
 * performed on the type seq_t, which may be an unsigned integer type
 * or an arbitrary‑precision integer class.
 */

#include <stdlib.h>

static const seq_t init_vals[14] = {
    0, 0, 0, 1,       /* a(0) – a(3)   */
    5, 18, 53, 135,   /* a(4) – a(7)   */
    305, 633, 1220,   /* a(8) – a(10)  */
    2217, 3834, 6359  /* a(11) – a(13) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* initialise cache with the 14 known values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 14);
        if (cache == NULL) {        /* allocation failure – abort */
            abort();
        }
        for (int i = 0; i < 14; ++i) {
            cache[i] = init_vals[i];
        }
        cached = 14;
    }

    /* answer directly if already computed */
    if ((size_t)n < cached) {
        return cache[(size_t)n];
    }

    /* enlarge cache up to the requested index */
    if (cached <= (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        /* entries cached[old..new-1] are uninitialised; we will fill them below */
    }

    /* compute missing values from the cache up to n */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        /* positive‑coefficient contributions */
        seq_t pos = 4 * cache[i - 1]          /* +4*a(n-1)        */
                  + 2 * cache[i - 4]          /* + 2*a(n-4)       */
                  + 4 * cache[i - 5]          /* + 4*a(n-5)       */
                  + 3 * cache[i - 6]          /* + 3*a(n-6)       */
                  + 3 * cache[i - 8]          /* + 3*a(n-8)       */
                  + 4 * cache[i - 9]          /* + 4*a(n-9)       */
                  + 2 * cache[i -10]          /* + 2*a(n-10)      */
                  + 4 * cache[i -13];         /* + 4*a(n-13)      */

        /* negative‑coefficient contributions */
        seq_t neg = 4 * cache[i - 2]          /* - 4*a(n-2)       */
                  + 2 * cache[i - 3]          /* - 2*a(n-3)       */
                  + 12 * cache[i - 7]         /* -12*a(n-7)       */
                  + 2 * cache[i -11]          /* - 2*a(n-11)      */
                  + 4 * cache[i -12]          /* - 4*a(n-12)      */
                  + 1 * cache[i -14];         /* -   a(n-14)      */

        cache[i] = pos - neg;                 /* guaranteed non‑negative */
    }
    cached = (size_t)n + 1;

    return cache[(size_t)n];
}
