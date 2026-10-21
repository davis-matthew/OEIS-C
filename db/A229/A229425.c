/*
 * A229425  Number of n X 5 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, rows
 *          lexicographically nondecreasing, and columns lexicographically
 *          nonincreasing.
 * https://oeis.org/A229425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) 
 *            + 462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) 
 *            + 55*a(n-9)  - 11*a(n-10) + a(n-11)    for n>11
 * Initial values (n=1..11):
 * 17, 102, 524, 2204, 7816, 24126, 66503, 166972, 387738, 842802, 1731129
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static const seq_t init_vals[11] = {
    (seq_t)17, (seq_t)102, (seq_t)524, (seq_t)2204, (seq_t)7816,
    (seq_t)24126, (seq_t)66503, (seq_t)166972, (seq_t)387738,
    (seq_t)842802, (seq_t)1731129
};

static seq_t *cache = NULL;
static size_t cache_size = 0;     /* number of terms currently stored */
static size_t cache_cap   = 0;     /* allocated capacity */

static void ensure_cache(idx_t n)
{
    idx_t needed = n; /* need to store terms 1..n inclusive, 0..n-1 indices */
    if ((size_t)needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < (size_t)needed)
        new_cap = (size_t)needed;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real library we would handle allocation failure. */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

static void compute_terms(idx_t n)
{
    /* Compute terms up to n (1-indexed) */
    for (idx_t i = (idx_t)(cache_size + 1); i <= n; ++i) {
        size_t idx = (size_t)(i - 1); /* zero-based index */
        seq_t val;
        if (i <= 11) {
            val = init_vals[(size_t)(i - 1)];
        } else {
            /* recurrence:
             * a(i) = 11*a(i-1) - 55*a(i-2) + 165*a(i-3) - 330*a(i-4)
             *        + 462*a(i-5) - 462*a(i-6) + 330*a(i-7) - 165*a(i-8)
             *        + 55*a(i-9)  - 11*a(i-10) + a(i-11)
             */
            seq_t t1 = 11 * cache[idx - 1];
            seq_t t2 = 55 * cache[idx - 2];
            seq_t t3 = 165 * cache[idx - 3];
            seq_t t4 = 330 * cache[idx - 4];
            seq_t t5 = 462 * cache[idx - 5];
            seq_t t6 = 462 * cache[idx - 6];
            seq_t t7 = 330 * cache[idx - 7];
            seq_t t8 = 165 * cache[idx - 8];
            seq_t t9 = 55 * cache[idx - 9];
            seq_t t10 = 11 * cache[idx - 10];
            seq_t t11 = cache[idx - 11];

            val = 0;
            val += t1;
            val -= t2;
            val += t3;
            val -= t4;
            val += t5;
            val -= t6;
            val += t7;
            val -= t8;
            val += t9;
            val -= t10;
            val += t11;
        }
        cache[idx] = val;
    }
    cache_size = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n==0, but safe to return 0 */

    if (cache == NULL)
        ensure_cache(16);           /* allocate initial space */

    if ((size_t)n > cache_cap)
        ensure_cache(n);

    if ((size_t)n > cache_size)
        compute_terms(n);

    return cache[(size_t)(n - 1)];
}
