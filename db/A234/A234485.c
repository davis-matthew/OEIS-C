/*
 * A234485  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code for OEIS sequence A234485
   Formula: a(n) = 13*a(n-1) -43*a(n-2) -107*a(n-3) +865*a(n-4)
             -1033*a(n-5) -1916*a(n-6) +2822*a(n-7) +2712*a(n-8)
             -1332*a(n-9) -1512*a(n-10) -288*a(n-11)
   Initial values (offset 1):
     a(1)=944, a(2)=3540, a(3)=13260, a(4)=49784, a(5)=187196,
     a(6)=705072, a(7)=2659824, a(8)=10049936, a(9)=38030588,
     a(10)=144134184, a(11)=547062036
 */

#include <stdlib.h>

static const seq_t init[11] = {
    944ULL,
    3540ULL,
    13260ULL,
    49784ULL,
    187196ULL,
    705072ULL,
    2659824ULL,
    10049936ULL,
    38030588ULL,
    144134184ULL,
    547062036ULL
};

static seq_t *cache = NULL;   /* cached values of a(k), k=1..size-1 */
static idx_t   cached = 0;    /* number of cached terms */

/* Computes a(n) with memoization; n >= 1  */
seq_t a(idx_t n)
{
    if (n <= 11) return init[n - 1];

    size_t N = (size_t)n;          /* safe as n >= 11 so fits into size_t */

    /* initialise cache if needed */
    if (!cache) {
        cache = (seq_t *)malloc((N > 11 ? N : 11) * sizeof(seq_t));
        if (!cache) abort();      /* memory allocation failed */
        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
        cached = 11;
    }
    /* grow cache if required */
    if (cached < N) {
        seq_t *tmp = (seq_t *)realloc(cache, N * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cached = N;
    }

    /* compute missing terms */
    for (size_t i = 11; i < N; ++i) {
        /* compute a(i+1) using recurrence.
           All indices are 0‑based. */
        seq_t val = cache[i - 1] * 13;           /* +13*a(n-1) */
        val += cache[i - 4] * 865;              /* +865*a(n-4) */
        val += cache[i - 7] * 2822;             /* +2822*a(n-7) */
        val += cache[i - 8] * 2712;             /* +2712*a(n-8) */

        val -= cache[i - 2] * 43;              /* -43*a(n-2) */
        val -= cache[i - 3] * 107;             /* -107*a(n-3) */
        val -= cache[i - 5] * 1033;            /* -1033*a(n-5) */
        val -= cache[i - 6] * 1916;            /* -1916*a(n-6) */
        val -= cache[i - 9] * 1332;            /* -1332*a(n-9) */
        val -= cache[i -10] * 1512;            /* -1512*a(n-10) */
        val -= cache[i -11] * 288;             /* -288*a(n-11) */

        cache[i] = val;
    }

    return cache[N - 1];
}
