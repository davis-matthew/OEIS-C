/*
 * A234230  Number of (n+1) X (3+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+1) × 4 0..7 arrays with every 2×2 subblock having
 *            its diagonal sum differing from its antidiagonal sum by 3. a(1)
 *            = 48592, a(2) = 635904, … a(14) = 18330899102362639544. The
 *            implementation uses a dynamic programming over the difference
 *            vectors of the rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define D 3375  /* 15^3 possible difference vectors (a,b,c) in [-7,7] */

static idx_t max_n_cached = 0;
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

static int diffA[D];
static int diffB[D];
static int diffC[D];
static seq_t w[D];
static int initialized = 0;

/* Precomputes all difference vectors, their weight w, and initial DP state. */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    dp_prev = (seq_t *)malloc(D * sizeof(seq_t));
    dp_next = (seq_t *)malloc(D * sizeof(seq_t));

    a_cache_cap = 2;
    a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
    a_cache[0] = 0; /* unused, offsets start at 1 */

    /* Build difference vectors and their weights */
    for (int a = -7; a <= 7; ++a)
        for (int b = -7; b <= 7; ++b)
            for (int c = -7; c <= 7; ++c)
            {
                int idx = ((a + 7) * 15 + (b + 7)) * 15 + (c + 7);
                diffA[idx] = a;
                diffB[idx] = b;
                diffC[idx] = c;

                /* Count rows with these differences */
                int lb = 0;
                if (a > lb) lb = a;
                int ab = a + b;
                if (ab > lb) lb = ab;
                int abc = ab + c;
                if (abc > lb) lb = abc;

                int ub = 7;
                int ua = 7 + a;
                if (ua < ub) ub = ua;
                int uab = 7 + a + b;
                if (uab < ub) ub = uab;
                int uabc = 7 + a + b + c;
                if (uabc < ub) ub = uabc;

                int cnt = ub - lb + 1;
                if (cnt < 0) cnt = 0;
                w[idx] = (seq_t)cnt;
            }

    /* Initial DP – one row */
    for (int i = 0; i < D; ++i)
        dp_prev[i] = w[i];
    max_n_cached = 0;
}

/* Ensures that the cache can store at least indices 0 … needed */
static void ensure_cache(idx_t needed)
{
    if ((size_t)(needed + 1) <= a_cache_cap) return;
    size_t newcap = a_cache_cap * 2;
    if (newcap < (size_t)(needed + 1))
        newcap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cache_cap = newcap;
}

/* Public function returning the n-th term */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined in the OEIS sequence, but safe */

    init();

    if (n <= max_n_cached)
        return a_cache[n];

    /* Extend the cache up to n */
    while (max_n_cached < n)
    {
        /* Zero next DP vector */
        for (int i = 0; i < D; ++i)
            dp_next[i] = 0;

        /* Transition */
        for (int i = 0; i < D; ++i)
        {
            seq_t val = dp_prev[i];
            if (!val) continue;

            int a = diffA[i];
            int b = diffB[i];
            int c = diffC[i];

            for (int mask = 0; mask < 8; ++mask)
            {
                int sa = (mask & 1) ? 3 : -3;
                int sb = (mask & 2) ? 3 : -3;
                int sc = (mask & 4) ? 3 : -3;

                int to_a = a - sa;
                int to_b = b - sb;
                int to_c = c - sc;

                if (to_a < -7 || to_a > 7 ||
                    to_b < -7 || to_b > 7 ||
                    to_c < -7 || to_c > 7)
                    continue;

                int to_idx = ((to_a + 7) * 15 + (to_b + 7)) * 15 + (to_c + 7);
                dp_next[to_idx] += val * w[to_idx];
            }
        }

        /* Sum all DP states to obtain the term for this n */
        seq_t total = 0;
        for (int i = 0; i < D; ++i)
            total += dp_next[i];

        /* Store in cache */
        ensure_cache(max_n_cached + 1);
        a_cache[max_n_cached + 1] = total;

        ++max_n_cached;

        /* Swap DP vectors for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    return a_cache[n];
}
