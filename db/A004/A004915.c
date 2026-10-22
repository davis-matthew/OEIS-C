/*
 * A004915  Numbers that are the sum of at most 9 positive 11th powers.
 * https://oeis.org/A004915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search — list every sum of at most 9 positive 11th powers below
 *            M^11 (bases < M), sort and dedupe, and increase M until there
 *            are at least n terms
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *s_list = NULL;
static size_t s_cnt = 0, s_cap = 0;
static uint64_t s_pw[64];
static int s_m = 0;
static uint64_t s_bound = 0;

static uint64_t *e_buf = NULL;
static size_t e_cnt = 0, e_cap = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u < v) ? -1 : (u > v) ? 1 : 0;
}

static void push_val(uint64_t v) {
    if (e_cnt >= e_cap) {
        size_t nc = e_cap ? e_cap * 2 : 4096;
        e_buf = (uint64_t *)realloc(e_buf, nc * sizeof(uint64_t));
        e_cap = nc;
    }
    e_buf[e_cnt++] = v;
}

static void enumerate(uint64_t sum, int idx, int rem) {
    push_val(sum);
    if (rem == 0) return;
    for (int i = idx; i < s_m; i++) {
        if (s_pw[i] >= s_bound - sum) break;
        enumerate(sum + s_pw[i], i, rem - 1);
    }
}

static uint64_t pow11(uint64_t b) {
    uint64_t p = 1;
    for (int e = 0; e < 11; e++) p *= b;
    return p;
}

static void build(int M) {
    /* bases 1..M-1, bound M^11 (M <= 56 keeps it within 64 bits) */
    s_m = M - 1;
    for (int i = 0; i < s_m; i++) s_pw[i] = pow11((uint64_t)(i + 1));
    s_bound = pow11((uint64_t)M);
    e_cnt = 0;
    enumerate(0, 0, 9);
    qsort(e_buf, e_cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < e_cnt; i++)
        if (u == 0 || e_buf[i] != e_buf[u - 1]) e_buf[u++] = e_buf[i];
    if (u > s_cap) {
        s_list = (uint64_t *)realloc(s_list, u * sizeof(uint64_t));
        s_cap = u;
    }
    for (size_t i = 0; i < u; i++) s_list[i] = e_buf[i];
    s_cnt = u;
}

static int s_M = 1;

seq_t a(idx_t n)
{
    while ((idx_t)s_cnt < n && s_M < 56) {
        s_M++;
        build(s_M);
    }
    if (n == 0 || n > (idx_t)s_cnt) return (seq_t)0;
    return u128_to_seq((unsigned __int128)s_list[n - 1]);
}
