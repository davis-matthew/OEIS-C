/*
 * A206121  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and having the same number of clockwise edge
 *          increases as its horizontal and vertical neighbors.
 * https://oeis.org/A206121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 13*a(n-2) - 9*a(n-3) - 50*a(n-4)
 *              - 43*a(n-5) + 4*a(n-6) - 4*a(n-8),   for n>9
 * Initial values:
 *   a(1)=50, a(2)=170, a(3)=558, a(4)=1906, a(5)=6386,
 *   a(6)=21940, a(7)=74714, a(8)=258496, a(9)=891586
 */

#include <stdlib.h>

/* cached sequence values: values[1]..[max_n]  (index 0 unused) */
static seq_t *cache = NULL;        /* dynamically grown array  */
static idx_t  max_n = 0;           /* largest index computed so far */
static size_t cache_cap = 0;       /* allocated capacity */

/* ensure cache has space for indices up to need (inclusive) */
static void ensure_cache(idx_t need)
{
    if (need + 1 > cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap : 16);
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* initial values for 1..9 */
static const seq_t init_vals[10] = {
    0, /* 0 unused */
    50ULL,      /* a(1) */
    170ULL,     /* a(2) */
    558ULL,     /* a(3) */
    1906ULL,    /* a(4) */
    6386ULL,    /* a(5) */
    21940ULL,   /* a(6) */
    74714ULL,   /* a(7) */
    258496ULL,  /* a(8) */
    891586ULL   /* a(9) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out‑of‑range value */
    ensure_cache(n);

    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init_vals[i];
            } else {
                /* compute a(i) via the recurrence */
                seq_t pos = 2 * cache[i - 1];
                pos += 13 * cache[i - 2];
                pos += 4 * cache[i - 6];

                seq_t neg = 9 * cache[i - 3];
                neg += 50 * cache[i - 4];
                neg += 43 * cache[i - 5];
                neg += 4 * cache[i - 8];

                cache[i] = pos - neg;   /* pos ≥ neg for all n */
            }
        }
        max_n = n;
    }
    return cache[n];
}
