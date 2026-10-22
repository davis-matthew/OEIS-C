/*
 * A005786  Number of 6-covers of an unlabeled n-set.
 * https://oeis.org/A005786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over S_6 acting on the 63 nonzero 6-bit
 *            columns: a(n) = (1/720) * Sum_{g in S_6} [x^n] Prod_{cycles c of
 *            g on nonzero columns} 1/(1 - x^|c|), grouped by the 11 cycle
 *            types of S_6 with their class sizes
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 25, u128 n <= 81,
 *            big any n
 * Helpers:   alloc_zero, cycle_lengths, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/cycle_lengths.h"

#define NLIMB 5 /* 32-bit limbs, arithmetic mod 2^160 for fixed-width builds */

static const int parts[11][7] = {
    {6,0}, {5,1,0}, {4,2,0}, {4,1,1,0}, {3,3,0}, {3,2,1,0},
    {3,1,1,1,0}, {2,2,2,0}, {2,2,1,1,0}, {2,1,1,1,1,0}, {1,1,1,1,1,1,0}
};
static const int csize[11] = {120, 144, 90, 90, 40, 120, 40, 15, 45, 15, 1};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void compute_fixed(seq_t *res, idx_t N)
{
    size_t sz = (size_t)(N + 1) * NLIMB;
    uint32_t *tot = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *ser = (uint32_t *)malloc(sz * sizeof(uint32_t));
    int lens[64];
    int t, c, nc, l;
    idx_t i;
    for (t = 0; t < 11; t++) {
        nc = cycle_lengths(parts[t], lens);
        memset(ser, 0, sz * sizeof(uint32_t));
        ser[0] = 1;
        for (c = 0; c < nc; c++) {
            idx_t k = (idx_t)lens[c];
            for (i = k; i <= N; i++) {
                uint32_t *d = ser + i * NLIMB;
                const uint32_t *s = ser + (i - k) * NLIMB;
                uint64_t carry = 0;
                for (l = 0; l < NLIMB; l++) {
                    uint64_t v = (uint64_t)d[l] + s[l] + carry;
                    d[l] = (uint32_t)v;
                    carry = v >> 32;
                }
            }
        }
        for (i = 0; i <= N; i++) {
            uint32_t *d = tot + i * NLIMB;
            const uint32_t *s = ser + i * NLIMB;
            uint64_t carry = 0;
            for (l = 0; l < NLIMB; l++) {
                uint64_t v = (uint64_t)d[l] + (uint64_t)s[l] * (uint64_t)csize[t] + carry;
                d[l] = (uint32_t)v;
                carry = v >> 32;
            }
        }
    }
    for (i = 0; i <= N; i++) {
        uint32_t *d = tot + i * NLIMB;
        uint64_t rem = 0;
        unsigned __int128 v = 0;
        for (l = NLIMB - 1; l >= 0; l--) {
            uint64_t cur = (rem << 32) | d[l];
            d[l] = (uint32_t)(cur / 720);
            rem = cur % 720;
        }
        for (l = 3; l >= 0; l--) v = (v << 32) | d[l];
        res[i] = u128_to_seq(v);
    }
    free(tot);
    free(ser);
}

static void compute_big(seq_t *res, idx_t N)
{
    seq_t *ser = alloc_zero(N + 1);
    int lens[64];
    int t, c, nc;
    idx_t i;
    for (t = 0; t < 11; t++) {
        nc = cycle_lengths(parts[t], lens);
        for (i = 0; i <= N; i++) ser[i] = 0;
        ser[0] = 1;
        for (c = 0; c < nc; c++) {
            idx_t k = (idx_t)lens[c];
            for (i = k; i <= N; i++) ser[i] += ser[i - k];
        }
        for (i = 0; i <= N; i++) res[i] += ser[i] * csize[t];
    }
    for (i = 0; i <= N; i++) res[i] /= 720;
    release(ser, N + 1);
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        idx_t N = n;
        seq_t *nc;
        if (N < 2 * cache_len) N = 2 * cache_len;
        if (N < 64) N = 64;
        nc = alloc_zero(N + 1);
        if (seq_is_big())
            compute_big(nc, N);
        else
            compute_fixed(nc, N);
        if (cache) release(cache, cache_len);
        cache = nc;
        cache_len = N + 1;
    }
    return cache[n];
}
