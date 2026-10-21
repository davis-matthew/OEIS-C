/*
 * A205209  Number of (n+1)X3 0..2 arrays with the determinants of 2X2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A205209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) -49*a(n-2) -97*a(n-3) +664*a(n-4) -980*a(n-5)
 *            +448*a(n-6) for n>13, a(1)=452, a(2)=3248, a(3)=23578,
 *            a(4)=203313, a(5)=1846145, a(6)=17014174, a(7)=157776973,
 *            a(8)=1467364433, a(9)=13666197180, a(10)=127366953026,
 *            a(11)=1187443362372, a(12)=11072379506700, a(13)=103253395267594
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Base values for 1 ≤ n ≤ 13.  Index 0 is unused. */
static const seq_t init[14] = {
    0ULL,               /* placeholder for index 0 */
    452ULL,             /* a(1) */
    3248ULL,            /* a(2) */
    23578ULL,           /* a(3) */
    203313ULL,          /* a(4) */
    1846145ULL,         /* a(5) */
    17014174ULL,        /* a(6) */
    157776973ULL,       /* a(7) */
    1467364433ULL,      /* a(8) */
    13666197180ULL,     /* a(9) */
    127366953026ULL,    /* a(10) */
    1187443362372ULL,   /* a(11) */
    11072379506700ULL,  /* a(12) */
    103253395267594ULL /* a(13) */
};

static seq_t *cache = NULL;   /* Cache of computed values, indices 0..cache_size-1 */
static idx_t  cache_size = 0; /* Number of valid slots in cache */

/* Return the n‑th term of sequence A205209 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; a(0) is unused */
        return 0;

    if (n <= 13)              /* use the pre‑computed table for the first 13 terms */
        return init[n];

    /* Initialise cache if it has not been created yet. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(14 * sizeof(seq_t));
        if (!cache)
            exit(1);          /* out of memory */
        cache_size = 14;      /* indices 0..13 are valid */
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if necessary. */
    if (n + 1 > cache_size) {
        idx_t old_size = cache_size;
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            exit(1);          /* out of memory */
        cache = tmp;
        cache_size = n + 1;

        /* Compute the new terms using the recurrence. */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum_pos = cache[i-1] * (seq_t)15
                          + cache[i-4] * (seq_t)664
                          + cache[i-6] * (seq_t)448;

            seq_t sum_neg = cache[i-2] * (seq_t)49
                          + cache[i-3] * (seq_t)97
                          + cache[i-5] * (seq_t)980;

            cache[i] = sum_pos - sum_neg;   /* subtraction is safe: sum_pos > sum_neg */
        }
    }

    return cache[n];
}
