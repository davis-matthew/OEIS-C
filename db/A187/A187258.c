/*
 * A187258  Number of UH^jD's for some j>0, in all peakless Motzkin paths of
 *          length n (here U=(1,1), D=(1,-1) and H=(1,0); can be easily
 *          expressed using RNA secondary structure terminology).
 * https://oeis.org/A187258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187258: Number of UH^jD's (j>0) in all peakless Motzkin paths of length n
   Recurrence (order 7):
     (-n+1)a(n) + (4n-7)a(n-1) + (-5n+16)a(n-2) + (5n-22)a(n-3)
     + (-5n+18)a(n-4) + (5n-24)a(n-5) + (-4n+25)a(n-6) + (n-7)a(n-7) = 0
   Initial values (n=0..6):
     0,0,0,1,3,7,17
   Computation is memoised for arbitrary call order.                          */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- Static cache ----------------------------------------------------- */
    static seq_t *cache = NULL;          /* array of sequence values      */
    static size_t  cap   = 0;            /* allocated size of cache       */
    static idx_t   last  = 0;            /* largest n for which cache is filled */

    /* --- Ensure cache contains base terms ------------------------------- */
    if (cache == NULL) {
        /* allocate space for at least 7 initial terms */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));

        /* base values a(0)..a(6) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)17;

        last = 6;
    }
    /* --------------------------------------------------------------------- */

    /* Return known value if already cached */
    if (n <= last) return cache[n];

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        int64_t c1 = 4 * (int64_t)i - 7;   /* coefficient for a(i-1) */
        int64_t c2 = -5 * (int64_t)i + 16; /* coefficient for a(i-2) */
        int64_t c3 = 5 * (int64_t)i - 22;  /* coefficient for a(i-3) */
        int64_t c4 = -5 * (int64_t)i + 18; /* coefficient for a(i-4) */
        int64_t c5 = 5 * (int64_t)i - 24;  /* coefficient for a(i-5) */
        int64_t c6 = -4 * (int64_t)i + 25; /* coefficient for a(i-6) */
        int64_t c7 = (int64_t)i - 7;       /* coefficient for a(i-7) */

        /* Compute numerator = sum Positives – sum Negatives */
        seq_t sumPos = 0;
        seq_t sumNeg = 0;

        /* a(i-1) */
        if (c1 > 0)
            sumPos += (seq_t)c1 * cache[i - 1];
        else
            sumNeg += (seq_t)(-c1) * cache[i - 1];

        /* a(i-2) */
        if (c2 > 0)
            sumPos += (seq_t)c2 * cache[i - 2];
        else
            sumNeg += (seq_t)(-c2) * cache[i - 2];

        /* a(i-3) */
        if (c3 > 0)
            sumPos += (seq_t)c3 * cache[i - 3];
        else
            sumNeg += (seq_t)(-c3) * cache[i - 3];

        /* a(i-4) */
        if (c4 > 0)
            sumPos += (seq_t)c4 * cache[i - 4];
        else
            sumNeg += (seq_t)(-c4) * cache[i - 4];

        /* a(i-5) */
        if (c5 > 0)
            sumPos += (seq_t)c5 * cache[i - 5];
        else
            sumNeg += (seq_t)(-c5) * cache[i - 5];

        /* a(i-6) */
        if (c6 > 0)
            sumPos += (seq_t)c6 * cache[i - 6];
        else
            sumNeg += (seq_t)(-c6) * cache[i - 6];

        /* a(i-7) */
        if (c7 > 0)
            sumPos += (seq_t)c7 * cache[i - 7];
        else
            sumNeg += (seq_t)(-c7) * cache[i - 7];

        /* Compute a(i) = (sumPos - sumNeg) / (i-1) */
        seq_t numerator = sumPos - sumNeg;          /* safe: sumPos >= sumNeg */
        seq_t denominator = (seq_t)(i - 1);         /* i>=7 so denominator >0 */
        cache[i] = numerator / denominator;
    }

    last = n;
    return cache[n];
}
