/*
 * A076967  a(1) = 1, a(n+1) is the smallest square greater than the n-th
 *          partial sum.
 * https://oeis.org/A076967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest square greater than the sum of a(1)…a(n‑1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(1) = 1, for n > 1, a(n) = (⌊√S⌋ + 1)² where S = Σ_{i=1}^{n-1} a(i) */

#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* Generic integer square‑root: floor(sqrt(x)) using only integer ops */
static seq_t floor_sqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        /* mid = ceil((low + high) / 2) */
        seq_t mid = low + ((high - low + 1) >> 1);
        if (mid == 0) {            /* avoid division by zero */
            high = mid;
            continue;
        }
        /* if mid*mid > x, then x / mid < mid */
        if (x / mid < mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* -------------------------------------------------------------------------- */
/* Sequence a(n) with memoisation */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *a_cache = NULL;     /* a_cache[1 .. max]            */
    static seq_t *sum_cache = NULL;   /* sum_cache[0 .. max]          */
    static idx_t  max_computed = 0;   /* largest n that we have values for */
    static size_t cache_size = 0;     /* allocated array size (>= max_computed + 1) */

    if (n == 0) return (seq_t)0;          /* undefined, harmless */

    /* initialise caches on first use */
    if (a_cache == NULL) {
        cache_size = 16;                  /* initial capacity */
        a_cache    = (seq_t *)malloc(cache_size * sizeof(seq_t));
        sum_cache  = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        /* sum_cache[0] = Σ up to index 0 == 0 */
        sum_cache[0] = (seq_t)0;
        max_computed = 0;
    }

    /* grow the arrays if needed */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (n + 1) * 2;
        a_cache   = (seq_t *)realloc(a_cache,   new_size * sizeof(seq_t));
        sum_cache = (seq_t *)realloc(sum_cache, (new_size + 1) * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute terms on demand */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t m = floor_sqrt_seq(sum_cache[i - 1]) + (seq_t)1;  /* ceil sqrt */
            seq_t ai = m * m;                                      /* square */
            a_cache[i]   = ai;
            sum_cache[i] = sum_cache[i - 1] + ai;
        }
        max_computed = n;
    }

    return a_cache[n];
}
