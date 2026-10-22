/*
 * A007416  The minimal numbers: sequence A005179 arranged in increasing
 *          order.
 * https://oeis.org/A007416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   no closed form. The terms are the distinct values of A005179(d),
 *            sorted. A DP over the primes 2,3,5,... finds best[d] = least
 *            number <= B with exactly d divisors. Each step allows p^e for
 *            every e >= 0, and the DP stops when the primorial exceeds B. The
 *            values are sorted, and the bound B is raised...
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 325, u64 n <= 3240,
 *            u128 n <= 8931, big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static uint64_t *t_key;
static seq_t *t_val;
static idx_t t_cnt, t_cap;
static idx_t *t_ht;
static size_t t_hcap;

static size_t t_hash(uint64_t d)
{
    uint64_t h = d * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(t_hcap - 1));
}

static void t_rehash(void)
{
    size_t i;
    idx_t j;
    free(t_ht);
    t_hcap *= 2;
    t_ht = (idx_t *)calloc(t_hcap, sizeof(idx_t));
    for (j = 0; j < t_cnt; j++) {
        i = t_hash(t_key[j]);
        while (t_ht[i] != 0) i = (i + 1) & (t_hcap - 1);
        t_ht[i] = j + 1;
    }
}

static void t_upsert(uint64_t d, seq_t w)
{
    size_t i = t_hash(d);
    while (t_ht[i] != 0) {
        idx_t pos = t_ht[i] - 1;
        if (t_key[pos] == d) {
            if (w < t_val[pos]) t_val[pos] = w;
            return;
        }
        i = (i + 1) & (t_hcap - 1);
    }
    if (t_cnt == t_cap) {
        idx_t nc = t_cap * 2, j;
        seq_t *nv = alloc_zero(nc);
        for (j = 0; j < t_cnt; j++) nv[j] = t_val[j];
        release(t_val, t_cap);
        t_val = nv;
        t_key = (uint64_t *)realloc(t_key, (size_t)nc * sizeof(uint64_t));
        t_cap = nc;
    }
    t_key[t_cnt] = d;
    t_val[t_cnt] = w;
    t_ht[i] = t_cnt + 1;
    t_cnt++;
    if (t_cnt * 2 > (idx_t)t_hcap) t_rehash();
}

static uint64_t next_prime(uint64_t p)
{
    for (;;) {
        uint64_t q, f;
        bool ok = true;
        p++;
        q = p;
        if (q < 2) continue;
        for (f = 2; f * f <= q; f++)
            if (q % f == 0) { ok = false; break; }
        if (ok) return q;
    }
}

static void build(seq_t B)
{
    seq_t prim = (seq_t)1;
    uint64_t p = 1;
    idx_t gap, i, j;

    t_cap = 1024;
    t_cnt = 0;
    t_key = (uint64_t *)malloc((size_t)t_cap * sizeof(uint64_t));
    t_val = alloc_zero(t_cap);
    t_hcap = 4096;
    t_ht = (idx_t *)calloc(t_hcap, sizeof(idx_t));
    t_upsert(1, (seq_t)1);

    for (;;) {
        seq_t Bp;
        idx_t c;
        seq_t *snap;
        p = next_prime(p);
        if (prim > B / p) break;
        prim *= p;
        Bp = B / p;
        c = t_cnt;
        snap = alloc_zero(c);
        for (j = 0; j < c; j++) snap[j] = t_val[j];
        for (j = 0; j < c; j++) {
            seq_t w = snap[j];
            uint64_t d = t_key[j];
            uint64_t e = 1;
            while (w <= Bp) {
                w *= p;
                t_upsert(d * (e + 1), w);
                e++;
            }
        }
        release(snap, c);
    }

    /* shell sort of the values */
    for (gap = t_cnt / 2; gap > 0; gap /= 2) {
        for (i = gap; i < t_cnt; i++) {
            seq_t tmp = t_val[i];
            j = i;
            while (j >= gap && t_val[j - gap] > tmp) {
                t_val[j] = t_val[j - gap];
                j -= gap;
            }
            t_val[j] = tmp;
        }
    }
    free(t_key);
    t_key = NULL;
    free(t_ht);
    t_ht = NULL;
}

static seq_t *memo = NULL;
static idx_t memo_len = 0, memo_cap = 0;
static unsigned memo_bits = 64;
static bool memo_full = false;

seq_t a(idx_t n)
{
    bool fixed = seq_is_fixed_width();
    if (memo != NULL && n <= memo_len) return memo[n - 1];
    for (;;) {
        seq_t B;
        if (memo != NULL) {
            if (fixed || memo_full) return (seq_t)0;
            release(memo, memo_cap);
            memo = NULL;
            memo_bits += 32;
        }
        if (fixed) B = (seq_t)0 - 1;
        else B = (seq_t)1 << memo_bits;
        build(B);
        memo = t_val;
        memo_len = t_cnt;
        memo_cap = t_cap;
        t_val = NULL;
        if (fixed) memo_full = true;
        if (n <= memo_len) return memo[n - 1];
    }
}
