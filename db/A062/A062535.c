/*
 * A062535  Don't be greedy! Difference between number of squares needed to
 *          sum to n using the greedy algorithm and using the best such sum.
 * https://oeis.org/A062535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 334, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A053610(n) - A002828(n)
 *   (number of squares used by the greedy algorithm minus the
 *    minimal number of squares summing to n)
 *
 * This implementation maintains a small dynamic cache of the
 * minimal square‑count DP up to the largest n requested.  For any
 * n it computes the greedy count by repeatedly subtracting the
 * largest possible square, then returns the difference.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root for idx_t (using binary search) */
static inline idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= (unsigned __int128)x)
        {
            ans = mid;
            lo  = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Count of squares used by the greedy algorithm for n. */
static seq_t greedy_count(idx_t n)
{
    seq_t remaining = (seq_t)n;
    seq_t cnt = 0;
    while (remaining != (seq_t)0)
    {
        idx_t r = isqrt((idx_t)remaining);
        seq_t sq = (seq_t)r * (seq_t)r;
        remaining -= sq;
        ++cnt;
    }
    return cnt;
}

/*
 * Main sequence function.  It keeps a static cache 'min_sq' containing
 * the minimal number of squares for all indices up to the largest n
 * requested so far.
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Static state */
    static seq_t *min_sq = NULL;    /* DP array */
    static size_t cache_size = 0;   /* allocated capacity */
    static idx_t  computed   = 0;   /* largest index computed */

    /* Initialise cache on first call */
    if (cache_size == 0)
    {
        cache_size = 1;                /* need index 0 */
        min_sq = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!min_sq) return (seq_t)0;  /* allocation failure */
        min_sq[0] = (seq_t)0;           /* 0 requires 0 squares */
        computed = 0;
    }

    /* Grow and fill the cache if necessary */
    if (computed < n)
    {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;

        seq_t *tmp = (seq_t *)realloc(min_sq, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        min_sq = tmp;
        cache_size = new_size;

        /* DP for each new index */
        for (idx_t i = computed + 1; i <= n; ++i)
        {
            seq_t best_val = (seq_t)n;  /* upper bound, harmless */
            for (idx_t j = 1; j * j <= i; ++j)
            {
                idx_t sq   = j * j;
                seq_t cand = min_sq[i - sq] + (seq_t)1;
                if (cand < best_val)
                    best_val = cand;
            }
            min_sq[i] = best_val;
        }
        computed = n;
    }

    /* Compute difference: greedy count minus minimal count */
    seq_t g = greedy_count(n);
    seq_t m = min_sq[n];
    return g - m;
}
