/*
 * A045109  Numbers whose base-4 representation contains exactly four 1's and
 *          four 2's.
 * https://oeis.org/A045109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th smallest positive integer whose base‑4
 *            representation contains exactly four 1's and four 2's
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gen_choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates numbers length by length, caches all 
   numbers found so far, and returns the requested element. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gen_choose.h"

/* Compute binomial coefficient n choose k for n <= 32.  The result fits
   in a 64‑bit unsigned integer. */
static uint64_t comb(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (unsigned i = 1; i <= k; ++i)
        res = res * (n - i + 1) / i;
    return res;
}

/* Comparator used by qsort for seq_t values. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Generate all numbers of length L into tmp; result is stored in outCnt. */
static void generate_length(int L, seq_t *tmp, size_t *outCnt)
{
    int positions[8];
    gen_choose(0, 0, 8, L, positions, tmp, outCnt);
}

/* Global cache storing all numbers generated so far. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static int   next_len    = 8;   /* next length to be processed */

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len >= n) return;

    while ((idx_t)cache_len < n) {
        int L = next_len++;
        size_t combL8 = comb(L, 8);
        size_t total_types = combL8 * 70ULL * ((size_t)1 << (L - 8));
        seq_t *tmp = (seq_t *)malloc(total_types * sizeof(seq_t));
        if (!tmp) abort();   /* Out of memory */

        size_t out_cnt = 0;
        generate_length(L, tmp, &out_cnt);

        /* Sort the elements of this length. */
        qsort(tmp, out_cnt, sizeof(seq_t), seq_cmp);

        /* Grow the global cache if necessary. */
        if (cache_cap < cache_len + out_cnt) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < cache_len + out_cnt) new_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Append to the cache. */
        for (size_t i = 0; i < out_cnt; ++i)
            cache[cache_len++] = tmp[i];
        free(tmp);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];
}
