/*
 * A099314  Exponent of greatest power of 3 dividing the n-th minimal number.
 * https://oeis.org/A099314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007949(A007416(n)). The minimal numbers A007416 are
 *            found by a DP over primes 2,3,5,... (least number <= 2^128-1
 *            with each divisor count). The values are sorted, and the result
 *            is the 3-adic valuation of the n-th one. The DP works in
 *            unsigned __int128 whatever seq_t is, so minimal numbers...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9575 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 mn_u128;

static uint64_t *mn_key;
static mn_u128 *mn_val;
static size_t mn_cnt, mn_cap;
static size_t *mn_ht;
static size_t mn_hcap;

static size_t mn_hash(uint64_t d)
{
    uint64_t h = d * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(mn_hcap - 1));
}

static void mn_rehash(void)
{
    size_t i, j;
    free(mn_ht);
    mn_hcap *= 2;
    mn_ht = (size_t *)calloc(mn_hcap, sizeof(size_t));
    for (j = 0; j < mn_cnt; j++) {
        i = mn_hash(mn_key[j]);
        while (mn_ht[i] != 0) i = (i + 1) & (mn_hcap - 1);
        mn_ht[i] = j + 1;
    }
}

static void mn_upsert(uint64_t d, mn_u128 w)
{
    size_t i = mn_hash(d);
    while (mn_ht[i] != 0) {
        size_t pos = mn_ht[i] - 1;
        if (mn_key[pos] == d) {
            if (w < mn_val[pos]) mn_val[pos] = w;
            return;
        }
        i = (i + 1) & (mn_hcap - 1);
    }
    if (mn_cnt == mn_cap) {
        mn_cap *= 2;
        mn_key = (uint64_t *)realloc(mn_key, mn_cap * sizeof(uint64_t));
        mn_val = (mn_u128 *)realloc(mn_val, mn_cap * sizeof(mn_u128));
    }
    mn_key[mn_cnt] = d;
    mn_val[mn_cnt] = w;
    mn_ht[i] = mn_cnt + 1;
    mn_cnt++;
    if (mn_cnt * 2 > mn_hcap) mn_rehash();
}

static uint64_t mn_next_prime(uint64_t p)
{
    for (;;) {
        uint64_t f;
        bool ok = true;
        p++;
        if (p < 2) continue;
        for (f = 2; f * f <= p; f++)
            if (p % f == 0) { ok = false; break; }
        if (ok) return p;
    }
}

static bool mn_built = false;

static void mn_build(void)
{
    mn_u128 B = ~(mn_u128)0;
    mn_u128 prim = 1;
    uint64_t p = 1;
    size_t gap, i, j;

    mn_cap = 1024;
    mn_cnt = 0;
    mn_key = (uint64_t *)malloc(mn_cap * sizeof(uint64_t));
    mn_val = (mn_u128 *)malloc(mn_cap * sizeof(mn_u128));
    mn_hcap = 4096;
    mn_ht = (size_t *)calloc(mn_hcap, sizeof(size_t));
    mn_upsert(1, (mn_u128)1);

    for (;;) {
        mn_u128 Bp;
        size_t c;
        mn_u128 *snap;
        p = mn_next_prime(p);
        if (prim > B / p) break;
        prim *= p;
        Bp = B / p;
        c = mn_cnt;
        snap = (mn_u128 *)malloc(c * sizeof(mn_u128));
        for (j = 0; j < c; j++) snap[j] = mn_val[j];
        for (j = 0; j < c; j++) {
            mn_u128 w = snap[j];
            uint64_t d = mn_key[j];
            uint64_t e = 1;
            while (w <= Bp) {
                w *= p;
                mn_upsert(d * (e + 1), w);
                e++;
            }
        }
        free(snap);
    }

    for (gap = mn_cnt / 2; gap > 0; gap /= 2) {
        for (i = gap; i < mn_cnt; i++) {
            mn_u128 tmp = mn_val[i];
            j = i;
            while (j >= gap && mn_val[j - gap] > tmp) {
                mn_val[j] = mn_val[j - gap];
                j -= gap;
            }
            mn_val[j] = tmp;
        }
    }
    free(mn_key);
    mn_key = NULL;
    free(mn_ht);
    mn_ht = NULL;
    mn_built = true;
}

seq_t a(idx_t n)
{
    mn_u128 m;
    seq_t count = 0;
    if (!mn_built) mn_build();
    if (n < 1 || n > (idx_t)mn_cnt) return (seq_t)0;
    m = mn_val[n - 1];
    while (m % 3 == 0) {
        count++;
        m /= 3;
    }
    return count;
}
