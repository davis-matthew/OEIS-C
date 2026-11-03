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
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates numbers length by length, caches all 
   numbers found so far, and returns the requested element. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Recursively choose 8 positions out of length L for the digits 1 or 2. */
static void gen_choose(int depth, int start, int left, int L,
                       int *positions, seq_t *tmp, size_t *out)
{
    if (left == 0) {
        bool chosen[12] = { false };
        for (int i = 0; i < 8; ++i) chosen[positions[i]] = true;

        int otherIdxs[12];
        int oc = 0;
        for (int i = 0; i < L; ++i)
            if (!chosen[i]) otherIdxs[oc++] = i;

        for (int mask1 = 0; mask1 < 256; ++mask1)
            if (__builtin_popcount((unsigned)mask1) == 4) {
                uint8_t digits[12];
                for (int i = 0; i < L; ++i) digits[i] = 0;
                for (int i = 0; i < 8; ++i)
                    digits[positions[i]] = (mask1 & (1 << i)) ? 1 : 2;

                int otherMaskCount = 1 << oc;
                for (int mask2 = 0; mask2 < otherMaskCount; ++mask2) {
                    for (int j = 0; j < oc; ++j)
                        digits[otherIdxs[j]] = (mask2 & (1 << j)) ? 3 : 0;
                    if (digits[0] == 0) continue;   /* leading digit must be non‑zero */

                    seq_t val = 0;
                    for (int d = 0; d < L; ++d)
                        val = val * 4 + digits[d];
                    tmp[(*out)++] = val;
                }
            }
        return;
    }

    for (int i = start; i <= L - left; ++i) {
        positions[depth] = i;
        gen_choose(depth + 1, i + 1, left - 1, L, positions, tmp, out);
    }
}

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
        seq_t *tmp = mem_new(seq_t, total_types);
        if (!tmp) abort();   /* Out of memory */

        size_t out_cnt = 0;
        generate_length(L, tmp, &out_cnt);

        /* Sort the elements of this length. */
        mem_qsort(tmp, out_cnt, sizeof(seq_t), seq_cmp);

        /* Grow the global cache if necessary. */
        if (cache_cap < cache_len + out_cnt) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < cache_len + out_cnt) new_cap *= 2;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Append to the cache. */
        for (size_t i = 0; i < out_cnt; ++i)
            cache[cache_len++] = tmp[i];
        mem_free(tmp);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];
}
