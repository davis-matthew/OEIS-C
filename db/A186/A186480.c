/*
 * A186480  Number of (n+1)X8 0..3 arrays with every 2X2 subblock commuting
 *          with each of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A186480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 18, u64 n <= 48, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186480
 * Empirical recurrence for n > 18:
 *     a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 6*a(n-4) - 3*a(n-5)
 * Initial values for 1 ≤ n ≤ 18 are hard‑coded below.
 */

#include <stdlib.h>

/* Initial values (index 0 unused) */
static const seq_t _initial[19] = {
    0ULL,          /* a(0) – unused */
    20079ULL,      /* a(1) */
    14492ULL,      /* a(2) */
    82632ULL,      /* a(3) */
    101305ULL,     /* a(4) */
    433513ULL,     /* a(5) */
    418177ULL,     /* a(6) */
    1849873ULL,    /* a(7) */
    1894849ULL,    /* a(8) */
    7687009ULL,    /* a(9) */
    8602033ULL,    /* a(10) */
    31645329ULL,   /* a(11) */
    39574001ULL,   /* a(12) */
    131802797ULL,  /* a(13) */
    181420169ULL,  /* a(14) */
    553069145ULL,  /* a(15) */
    820669641ULL,  /* a(16) */
    2331052105ULL, /* a(17) */
    3678141757ULL  /* a(18) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1 – return 0 */

    /* Note: idx_t is an unsigned 64‑bit type; size_t is guaranteed
       to be able to hold indices up to the allocated cache size. */
    static seq_t *cache = NULL;   /* cache[0]..cache[cap-1] */
    static size_t cap = 0;        /* current capacity */

    /* First call: allocate space for initial values. */
    if (!cache) {
        cap = 19;                /* indices 0..18 inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure – unlikely */
        for (size_t i = 0; i < cap; ++i)
            cache[i] = _initial[i];
    }

    /* Expand cache if needed.  Casting n to size_t is safe for n ≤ ~9e18
       on typical 64‑bit systems; the sequence will be requested only up
       to 10 000 in the tests. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;

        /* Compute missing terms up to n (inclusive). */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            if (i <= 18) continue;        /* already initialized */
            /* a(i) = 2*a(i-1) + 2*a(i-2) - 6*a(i-3) + 6*a(i-4) - 3*a(i-5) */
            seq_t term = cache[i-1] + cache[i-1];          /* 2*a(i-1) */
            term += cache[i-2] + cache[i-2];              /* +2*a(i-2) */
            term += cache[i-4] * 6ULL;                   /* +6*a(i-4) */
            term -= cache[i-3] * 6ULL;                   /* -6*a(i-3) */
            term -= cache[i-5] * 3ULL;                   /* -3*a(i-5) */
            cache[i] = term;
        }
        cap = new_cap;
    }

    return cache[n];
}
