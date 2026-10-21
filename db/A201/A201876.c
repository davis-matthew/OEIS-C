/*
 * A201876  Number of zero-sum -n..n arrays of 5 elements with first and
 *          second differences also in -n..n.
 * https://oeis.org/A201876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) -a(n-2) +2*a(n-3) -a(n-4) +2*a(n-5) -a(n-6)
 *            -a(n-8) -a(n-9) +a(n-10) -2*a(n-11) +3*a(n-12) -3*a(n-13)
 *            +3*a(n-14) -3*a(n-15) +2*a(n-16) -a(n-17) +a(n-18) +a(n-19)
 *            +a(n-21) -2*a(n-22) +a(n-23) -2*a(n-24) +a(n-25) -a(n-26)
 *            +a(n-27); Initial values (computed by enumeration for n ≤
 *            27):...
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* #include <stdio.h>  /* for debugging only, not used in final build */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;            /* memoized sequence values */
static size_t cache_size = 0;          /* allocated array length   */
static idx_t cache_max = 0;            /* highest n fully computed */

/* Enumerate all valid arrays for n ≤ 27 */
static seq_t compute_small(idx_t n_limit)
{
    seq_t cnt = 0;
    int n = (int)n_limit;               /* safe: n_limit ≤ 27 */
    for (int d0 = -n; d0 <= n; ++d0) {
        for (int d1 = -n; d1 <= n; ++d1) {
            int a1 = d0 + d1;
            if (a1 < -n || a1 > n) continue;

            /* d2 bounds */
            int d2_min = -n;
            int d2_max =  n;
            int tmp = d1 - n;
            if (tmp > d2_min) d2_min = tmp;
            tmp = d1 + n;
            if (tmp < d2_max) d2_max = tmp;
            int a2_min = -n - d0 - d1;
            if (a2_min > d2_min) d2_min = a2_min;
            int a2_max =  n - d0 - d1;
            if (a2_max < d2_max) d2_max = a2_max;
            if (d2_min > d2_max) continue;

            for (int d2 = d2_min; d2 <= d2_max; ++d2) {
                /* d3 bounds */
                int d3_min = -n;
                int d3_max =  n;
                tmp = d2 - n;
                if (tmp > d3_min) d3_min = tmp;
                tmp = d2 + n;
                if (tmp < d3_max) d3_max = tmp;
                int a2 = d0 + d1 + d2;
                int a3_min = -n - a2;
                if (a3_min > d3_min) d3_min = a3_min;
                int a3_max =  n - a2;
                if (a3_max < d3_max) d3_max = a3_max;
                if (d3_min > d3_max) continue;

                for (int d3 = d3_min; d3 <= d3_max; ++d3) {
                    /* compute d4 from equality */
                    int d4 = -(5*d0 + 4*d1 + 3*d2 + 2*d3);
                    if (d4 < -n || d4 > n) continue;
                    int diff = d4 - d3;
                    if (diff < -n || diff > n) continue;

                    int a4 = d0 + d1 + d2 + d3 + d4;
                    if (a4 < -n || a4 > n) continue;

                    ++cnt;                 /* valid array found */
                }
            }
        }
    }
    return cnt;
}

/* Ensure that cache has values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_size = 64;
        cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
    }
    while (cache_size <= (size_t)n) {
        cache_size <<= 1;
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    }

    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t val;
        if (m <= 27) {
            val = compute_small(m);
        } else {
            val = 0;
            /* recurrence according to OEIS entry               */
            val += cache[m-1];
            val -= cache[m-2];
            val += (seq_t)2 * cache[m-3];
            val -= cache[m-4];
            val += (seq_t)2 * cache[m-5];
            val -= cache[m-6];
            val -= cache[m-8];
            val -= cache[m-9];
            val += cache[m-10];
            val -= (seq_t)2 * cache[m-11];
            val += (seq_t)3 * cache[m-12];
            val -= (seq_t)3 * cache[m-13];
            val += (seq_t)3 * cache[m-14];
            val -= (seq_t)3 * cache[m-15];
            val += (seq_t)2 * cache[m-16];
            val -= cache[m-17];
            val += cache[m-18];
            val += cache[m-19];
            val += cache[m-21];
            val -= (seq_t)2 * cache[m-22];
            val += cache[m-23];
            val -= (seq_t)2 * cache[m-24];
            val += cache[m-25];
            val -= cache[m-26];
            val += cache[m-27];
        }
        cache[m] = val;
    }
    cache_max = n;
}

/* Function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence offset is 1; 0 is unused */
    ensure_cache(n);
    return cache[n];
}
