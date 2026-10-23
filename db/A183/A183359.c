/*
 * A183359  One quarter the number of nX7 1..4 arrays with no two neighbors of
 *          any element equal to each other.
 * https://oeis.org/A183359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 45, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A183359
 *  Formula: a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) + 4*a(n-5) + a(n-6) - a(n-9)
 *  for n > 14
 *  Base values (n = 1 … 14):
 *      972, 36864, 34992, 63504, 137088, 331776, 829440,
 *      1806336, 3852288, 8294400, 18137088, 40144896,
 *      88897536, 196448256
 *
 *  The function a(idx_t n) returns the n‑th term, with offset 1.
 *  It memoises results so that calls in any order remain efficient.
 */

#include <stdlib.h>

static const seq_t base_values[14] = {
    972ULL,          /* a(1) */
    36864ULL,        /* a(2) */
    34992ULL,        /* a(3) */
    63504ULL,        /* a(4) */
    137088ULL,       /* a(5) */
    331776ULL,       /* a(6) */
    829440ULL,       /* a(7) */
    1806336ULL,      /* a(8) */
    3852288ULL,      /* a(9) */
    8294400ULL,      /* a(10) */
    18137088ULL,     /* a(11) */
    40144896ULL,     /* a(12) */
    88897536ULL,     /* a(13) */
    196448256ULL     /* a(14) */
};

static seq_t *cache = NULL;     /* memoised a(n) for indices 0 … max_n */
static size_t capacity = 0;     /* allocated size (>= max_n + 1) */
static idx_t  max_n = 0;        /* maximum index currently stored */

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* OEIS offset is 1; return 0 for n==0 */
        return (seq_t)0;
    }

    /* Expand cache if required */
    if (capacity <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise base values once */
    if (max_n < 14) {
        if (capacity < 15) {      /* need indices 0 … 14 */
            size_t new_cap = 15;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            capacity = new_cap;
        }
        /* cache[0] is unused, can be left 0 */
        for (idx_t i = 1; i <= 14; ++i) {
            cache[i] = base_values[i - 1];
        }
        max_n = 14;
    }

    /* Return cached value if available */
    if (n <= max_n) {
        return cache[n];
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + a(i-3) - 2*a(i-4) + 4*a(i-5) + a(i-6) - a(i-9) */
        seq_t term1 = cache[i - 1];
        term1 += cache[i - 1];          /* 2*a(i-1) */
        term1 += cache[i - 3];          /* + a(i-3) */

        seq_t term4 = cache[i - 5];
        term4 += term4;                 /* 2*a(i-5) */
        term4 += term4;                 /* 4*a(i-5) */
        term1 += term4;                 /* +4*a(i-5) */

        term1 += cache[i - 6];          /* + a(i-6) */

        seq_t term2 = cache[i - 4];
        term2 += term2;                 /* 2*a(i-4) */
        term2 += cache[i - 9];          /* + a(i-9) */

        cache[i] = term1 - term2;       /* final result */
    }

    max_n = n;
    return cache[n];
}
