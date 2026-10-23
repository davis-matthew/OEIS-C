/*
 * A009993  Numbers whose decimal digits are in strictly increasing order.
 * https://oeis.org/A009993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 120, u32 n <= 512, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009993: numbers whose decimal digits are strictly increasing. */
/* The sequence consists of the 512 numbers obtained by taking each non‑empty
   subset of {1,…,9}, ordering the digits increasingly, and prefixing 0
   for the empty subset.  The numbers are returned in ascending order. */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <algorithm>
#endif

static seq_t *cache   = NULL;  /* sorted array of sequence values     */
static idx_t  cache_n = 0;     /* number of values stored (512)       */
static bool   init    = false; /* cache initialization flag           */

static int cmp_seqt(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

static void build_cache(void)
{
    const idx_t N = 512;                 /* 2^9 subsets of {1,…,9}          */
    seq_t *arr = (seq_t *)malloc(N * sizeof(seq_t));
    if (!arr) return; /* should never happen */

    /* Generate a number for each subset mask (0 … 511) */
    for (idx_t mask = 0; mask < N; ++mask) {
        seq_t val = 0;
        for (int d = 1; d <= 9; ++d) {
            if (mask & (1ULL << (d - 1)))
                val = val * 10 + d;
        }
        arr[mask] = val;
    }

    /* Sort the numbers in ascending order */
#ifdef __cplusplus
    std::sort(arr, arr + N, [](const seq_t &x, const seq_t &y) { return x < y; });
#else
    qsort(arr, N, sizeof(seq_t), cmp_seqt);
#endif

    cache   = arr;
    cache_n = N;
    init    = true;
}

seq_t a(idx_t n)
{
    if (!init) build_cache();

    if (n == 0) return 0;            /* defined behaviour for n < 1 */
    if (n <= cache_n) return cache[n - 1];
    return 0;                        /* sequence is undefined beyond 512 */
}
