/*
 * A004192  Numbers obtained by reversing digits of factorial numbers.
 * https://oeis.org/A004192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n+1)-th smallest of the numbers R(k!), k >= 0, counted
 *            with multiplicity, where R reverses the decimal digits (dropping
 *            leading zeros). Lengths of R(k!) are computed for all k, the
 *            values are sorted by (length, value), and only entries that tie
 *            in length are evaluated exactly
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void mul_limbs(uint32_t **L, size_t *len, size_t *cap, uint64_t k) {
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; i++) {
        uint64_t cur = (uint64_t)(*L)[i] * k + carry;
        (*L)[i] = (uint32_t)(cur % 1000000000ULL);
        carry = cur / 1000000000ULL;
    }
    while (carry) {
        if (*len == *cap) {
            *cap *= 2;
            *L = (uint32_t *)realloc(*L, *cap * sizeof(uint32_t));
        }
        (*L)[(*len)++] = (uint32_t)(carry % 1000000000ULL);
        carry /= 1000000000ULL;
    }
}

static seq_t rev_fact(idx_t k) {
    size_t cap = 16, len = 1;
    uint32_t *L = (uint32_t *)malloc(cap * sizeof(uint32_t));
    L[0] = 1;
    for (idx_t j = 2; j <= k; j++) mul_limbs(&L, &len, &cap, (uint64_t)j);
    seq_t r = 0;
    bool started = false;
    uint32_t chunk = 0, mult = 1;
    for (size_t i = 0; i < len; i++) {
        uint32_t v = L[i];
        int nd = 9;
        if (i == len - 1) {
            nd = 0;
            uint32_t t = v;
            while (t) { nd++; t /= 10; }
        }
        for (int j = 0; j < nd; j++) {
            uint32_t d = v % 10;
            v /= 10;
            if (!started && d == 0) continue;
            started = true;
            chunk = chunk * 10 + d;
            mult *= 10;
            if (mult == 1000000000U) {
                r = r * (seq_t)mult + (seq_t)chunk;
                chunk = 0; mult = 1;
            }
        }
    }
    if (mult > 1) r = r * (seq_t)mult + (seq_t)chunk;
    free(L);
    return r;
}

static long long rev_len(idx_t k, const uint32_t *L, size_t len) {
    long long digs = (long long)(len - 1) * 9;
    uint32_t t = L[len - 1];
    while (t) { digs++; t /= 10; }
    long long z = 0;
    for (uint64_t p = 5; p <= k; p *= 5) z += (long long)(k / p);
    return digs - z;
}

typedef struct { long long d; unsigned long long k; } dk_t;

static uint32_t *F = NULL;      /* Fk! in base 1e9, little endian */
static size_t Flen = 0, Fcap = 0;
static idx_t Fk = 0;
static long long *Dt = NULL;    /* Dt[k] = number of digits of R(k!) */
static size_t Dcap = 0;

static void extend(idx_t K) {
    if (F == NULL) {
        Fcap = 16; Flen = 1;
        F = (uint32_t *)malloc(Fcap * sizeof(uint32_t));
        F[0] = 1; Fk = 0;
        Dcap = 64;
        Dt = (long long *)malloc(Dcap * sizeof(long long));
        Dt[0] = 1;
    }
    while (Fk < K) {
        Fk++;
        mul_limbs(&F, &Flen, &Fcap, (uint64_t)Fk);
        if ((size_t)Fk >= Dcap) {
            while ((size_t)Fk >= Dcap) Dcap *= 2;
            Dt = (long long *)realloc(Dt, Dcap * sizeof(long long));
        }
        Dt[Fk] = rev_len(Fk, F, Flen);
    }
}

static int cmp_dk(const void *x, const void *y) {
    const dk_t *p = (const dk_t *)x, *q = (const dk_t *)y;
    if (p->d != q->d) return p->d < q->d ? -1 : 1;
    if (p->k != q->k) return p->k < q->k ? -1 : 1;
    return 0;
}

seq_t a(idx_t n) {
    extend(n + 40);
    dk_t *P = NULL;
    size_t cnt;
    for (;;) {
        cnt = (size_t)Fk + 1;
        P = (dk_t *)realloc(P, cnt * sizeof(dk_t));
        for (size_t k = 0; k < cnt; k++) { P[k].d = Dt[k]; P[k].k = k; }
        qsort(P, cnt, sizeof(dk_t), cmp_dk);
        long long Dn = P[n].d;
        bool ok = true;
        for (idx_t k = Fk - 19; k <= Fk; k++)
            if (Dt[k] <= Dn + 5) { ok = false; break; }
        if (ok) break;
        extend(Fk + 40);
    }
    long long Dn = P[n].d;
    size_t lo = (size_t)n, hi = (size_t)n + 1;
    while (lo > 0 && P[lo - 1].d == Dn) lo--;
    while (hi < cnt && P[hi].d == Dn) hi++;
    size_t g = hi - lo;
    seq_t *vals = mem_new(seq_t, g);
    for (size_t i = 0; i < g; i++) vals[i] = rev_fact((idx_t)P[lo + i].k);
    for (size_t i = 1; i < g; i++) {
        seq_t v = vals[i];
        size_t j = i;
        while (j > 0 && vals[j - 1] > v) { vals[j] = vals[j - 1]; j--; }
        vals[j] = v;
    }
    seq_t res = vals[(size_t)n - lo];
    mem_free(vals);
    free(P);
    return res;
}
