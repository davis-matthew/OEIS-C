/*
 * A219137  Unchanging value maps: number of n X 3 binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 n X 3 array.
 * https://oeis.org/A219137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) + a(n-3) - 12*a(n-5) - 10*a(n-6) +
 *            2*a(n-7) + 4*a(n-8) Initial values: a(1)=4, a(2)=18, a(3)=78,
 *            a(4)=331, a(5)=1400, a(6)=5949, a(7)=25277, a(8)=107353
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[8] = {4,18,78,331,1400,5949,25277,107353};

/* Cache for previously computed terms */
static seq_t *cache = NULL;        /* array of computed a(n) */
static size_t cache_cap = 0;       /* current capacity of cache */
static idx_t max_comp = 0;         /* maximum n computed so far (>=0) */

/* Ensure the cache has computed terms up to n (n>=1) */
static void ensure_cache_upto(idx_t n)
{
    if (n <= max_comp)
        return;                   /* already computed */

    /* Allocate or grow cache if needed */
    if (cache == NULL) {
        /* initial allocation: at least 8 but at least n */
        cache_cap = (n < 16) ? 16 : n;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* out of memory */

        /* initialise first 8 terms */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = base_vals[i];
        max_comp = 8;
    } else if ((idx_t)cache_cap < n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap < n)
            new_cap <<= 1;           /* double until large enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();  /* out of memory */
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {
            val = base_vals[i-1];
        } else {
            /* recurrence for i >= 9 */
            seq_t a1 = cache[i-2];   /* a(i-1) */
            seq_t a2 = cache[i-3];   /* a(i-2) */
            seq_t a3 = cache[i-4];   /* a(i-3) */
            seq_t a5 = cache[i-6];   /* a(i-5) */
            seq_t a6 = cache[i-7];   /* a(i-6) */
            seq_t a7 = cache[i-8];   /* a(i-7) */
            seq_t a8 = cache[i-9];   /* a(i-8) */

            seq_t t1 = (seq_t)4 * a1;
            t1 += a2;
            t1 += a3;
            seq_t t2 = (seq_t)12 * a5;
            t2 += (seq_t)10 * a6;
            seq_t t3 = (seq_t)2 * a7;
            t3 += (seq_t)4 * a8;

            val = t1 - t2 + t3;
        }
        cache[i-1] = val;
    }
    max_comp = n;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* behaviour below offset not important */
    if (n < 1)
        return 0;
    ensure_cache_upto(n);
    return cache[n-1];
}
