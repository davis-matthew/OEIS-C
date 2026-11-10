/*
 * A079717  Subminimal numbers, from minimal numbers by analogy with
 *          subfactorials.
 * https://oeis.org/A079717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A007416(n)/e + 1/2), computed exactly with a
 *            continued-fraction convergent of e (no floating point)
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A007416_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void A007416_release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static bool A007416_seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static uint64_t *A007416_t_key;
static seq_t *A007416_t_val;
static idx_t A007416_t_cnt, A007416_t_cap;
static idx_t *A007416_t_ht;
static size_t A007416_t_hcap;

static size_t A007416_t_hash(uint64_t d)
{
    uint64_t h = d * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(A007416_t_hcap - 1));
}

static void A007416_t_rehash(void)
{
    size_t i;
    idx_t j;
    free(A007416_t_ht);
    A007416_t_hcap *= 2;
    A007416_t_ht = (idx_t *)calloc(A007416_t_hcap, sizeof(idx_t));
    for (j = 0; j < A007416_t_cnt; j++) {
        i = A007416_t_hash(A007416_t_key[j]);
        while (A007416_t_ht[i] != 0) i = (i + 1) & (A007416_t_hcap - 1);
        A007416_t_ht[i] = j + 1;
    }
}

static void A007416_t_upsert(uint64_t d, seq_t w)
{
    size_t i = A007416_t_hash(d);
    while (A007416_t_ht[i] != 0) {
        idx_t pos = A007416_t_ht[i] - 1;
        if (A007416_t_key[pos] == d) {
            if (w < A007416_t_val[pos]) A007416_t_val[pos] = w;
            return;
        }
        i = (i + 1) & (A007416_t_hcap - 1);
    }
    if (A007416_t_cnt == A007416_t_cap) {
        idx_t nc = A007416_t_cap * 2, j;
        seq_t *nv = A007416_alloc_zero(nc);
        for (j = 0; j < A007416_t_cnt; j++) nv[j] = A007416_t_val[j];
        A007416_release(A007416_t_val, A007416_t_cap);
        A007416_t_val = nv;
        A007416_t_key = (uint64_t *)realloc(A007416_t_key, (size_t)nc * sizeof(uint64_t));
        A007416_t_cap = nc;
    }
    A007416_t_key[A007416_t_cnt] = d;
    A007416_t_val[A007416_t_cnt] = w;
    A007416_t_ht[i] = A007416_t_cnt + 1;
    A007416_t_cnt++;
    if (A007416_t_cnt * 2 > (idx_t)A007416_t_hcap) A007416_t_rehash();
}

static uint64_t A007416_next_prime(uint64_t p)
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

static void A007416_build(seq_t B)
{
    seq_t prim = (seq_t)1;
    uint64_t p = 1;
    idx_t gap, i, j;

    A007416_t_cap = 1024;
    A007416_t_cnt = 0;
    A007416_t_key = (uint64_t *)malloc((size_t)A007416_t_cap * sizeof(uint64_t));
    A007416_t_val = A007416_alloc_zero(A007416_t_cap);
    A007416_t_hcap = 4096;
    A007416_t_ht = (idx_t *)calloc(A007416_t_hcap, sizeof(idx_t));
    A007416_t_upsert(1, (seq_t)1);

    for (;;) {
        seq_t Bp;
        idx_t c;
        seq_t *snap;
        p = A007416_next_prime(p);
        if (prim > B / p) break;
        prim *= p;
        Bp = B / p;
        c = A007416_t_cnt;
        snap = A007416_alloc_zero(c);
        for (j = 0; j < c; j++) snap[j] = A007416_t_val[j];
        for (j = 0; j < c; j++) {
            seq_t w = snap[j];
            uint64_t d = A007416_t_key[j];
            uint64_t e = 1;
            while (w <= Bp) {
                w *= p;
                A007416_t_upsert(d * (e + 1), w);
                e++;
            }
        }
        A007416_release(snap, c);
    }

    for (gap = A007416_t_cnt / 2; gap > 0; gap /= 2) {
        for (i = gap; i < A007416_t_cnt; i++) {
            seq_t tmp = A007416_t_val[i];
            j = i;
            while (j >= gap && A007416_t_val[j - gap] > tmp) {
                A007416_t_val[j] = A007416_t_val[j - gap];
                j -= gap;
            }
            A007416_t_val[j] = tmp;
        }
    }
    free(A007416_t_key);
    A007416_t_key = NULL;
    free(A007416_t_ht);
    A007416_t_ht = NULL;
}

static seq_t *A007416_memo = NULL;
static idx_t A007416_memo_len = 0, A007416_memo_cap = 0;
static unsigned A007416_memo_bits = 64;
static bool A007416_memo_full = false;

static seq_t A007416_a(idx_t n)
{
    bool fixed = A007416_seq_is_fixed_width();
    if (A007416_memo != NULL && n <= A007416_memo_len) return A007416_memo[n - 1];
    for (;;) {
        seq_t B;
        if (A007416_memo != NULL) {
            if (fixed || A007416_memo_full) return (seq_t)0;
            A007416_release(A007416_memo, A007416_memo_cap);
            A007416_memo = NULL;
            A007416_memo_bits += 32;
        }
        if (fixed) B = (seq_t)0 - 1;
        else B = (seq_t)1 << A007416_memo_bits;
        A007416_build(B);
        A007416_memo = A007416_t_val;
        A007416_memo_len = A007416_t_cnt;
        A007416_memo_cap = A007416_t_cap;
        A007416_t_val = NULL;
        if (fixed) A007416_memo_full = true;
        if (n <= A007416_memo_len) return A007416_memo[n - 1];
    }
}

/* floor(m/e + 1/2) using the largest convergent P/Q of e with 3P <= limit */
static seq_t round_m_over_e(seq_t m)
{
    seq_t limit, pprev = 1, qprev = 0, pc = 2, qc = 1;
    seq_t Q = 0, R = 0, x;
    int nb = 0, i;
    idx_t k = 1;

    if (A007416_seq_is_fixed_width()) limit = ((seq_t)0 - 1) / 3;
    else limit = (seq_t)1 << 512;

    for (;;) {
        idx_t aa = (k % 3 == 2) ? 2 * ((k + 1) / 3) : 1;
        seq_t pn, qn;
        if (pc > (limit - pprev) / (seq_t)aa) break;
        pn = (seq_t)aa * pc + pprev;
        qn = (seq_t)aa * qc + qprev;
        pprev = pc; qprev = qc;
        pc = pn; qc = qn;
        k++;
    }

    x = m;
    while (x > 0) { x >>= 1; nb++; }
    for (i = nb - 1; i >= 0; i--) {
        Q = Q * 2;
        R = R * 2;
        if (((m >> i) & 1) != 0) R += qc;
        while (R >= pc) { R -= pc; Q++; }
    }
    if (R * 2 >= pc) Q++;
    return Q;
}

seq_t a(idx_t n)
{
    seq_t m = A007416_a(n);
    if (m == 0) return (seq_t)0;
    return round_m_over_e(m);
}
