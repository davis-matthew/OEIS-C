/*
 * A151566  Leftist toothpicks (see Comments for definition).
 * https://oeis.org/A151566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 2 * Σ_{k=0}^{n-1} 2^{popcount(k)} * a(2n+1) = a(2n) +
 *            2^{popcount(n)}
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute Σ_{k=0}^{m-1} 2^{popcount(k)} on demand, memoised. */
#include <stdlib.h>
#include <stdint.h>

/* static cache of partial sums g[m] = Σ_{k=0}^{m-1} 2^{popcount(k)} */
static seq_t *g_cache   = NULL;   /* array of g values */
static idx_t  g_last    = 0;       /* highest index whose g value is computed */

/* ensure that g_cache is large enough and contains values up to 'upto' */
static void ensure_g(idx_t upto)
{
    /* if already computed, nothing to do */
    if (g_cache != NULL && upto <= g_last)
        return;

    /* required capacity */
    size_t new_cap = (size_t) (upto + 1);   /* indices 0..upto inclusive */

    if (g_cache == NULL) {
        /* first allocation */
        g_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!g_cache) abort();                /* allocation failure */
        g_cache[0] = 0;                       /* g(0) = 0 */
    } else if (new_cap > (size_t)g_last + 1) {
        /* grow the array */
        g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!g_cache) abort();
    }

    /* compute new entries */
    size_t start = (size_t)g_last + 1;
    for (size_t i = start; i <= upto; ++i) {
        unsigned int pc = __builtin_popcountll((unsigned long long)(i - 1));
        /* 2^{popcount(i-1)} computed in 64‑bit and cast to seq_t */
        seq_t term = (seq_t)((unsigned long long)1 << pc);
        g_cache[i] = g_cache[i - 1] + term;
    }

    g_last = upto;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */

    idx_t half = n >> 1;            /* floor(n/2) */
    ensure_g(half);                 /* need g(half) = Σ_{k=0}^{half-1} 2^{popcount(k)} */

    /* a(2n) part: 2 * g(half) */
    seq_t result = (seq_t)2 * g_cache[half];

    if (n & 1) {                    /* odd n: add 2^{popcount(half)} */
        unsigned int pc = __builtin_popcountll((unsigned long long)half);
        seq_t term = (seq_t)((unsigned long long)1 << pc);
        result += term;
    }

    return result;
}
