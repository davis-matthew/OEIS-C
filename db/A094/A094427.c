/*
 * A094427  Terms of A094302 which are not squarefree, without repetition.
 * https://oeis.org/A094427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. Run the recurrence of A094302: a(1) = 1; a(m) = m*a(m-1)
 *            if m > a(m-1), otherwise a(m) = a(m-1)/gcd(a(m-1), m). a(m-1) is
 *            stored as its prime factorization, so huge values cost nothing.
 *            Each value that is not squarefree is recorded the first time it
 *            appears
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint32_t *A094427_spf = 0;   /* smallest prime factor */
static uint32_t *A094427_exp = 0;   /* exponent of prime p in current A094302 value */
static uint32_t *A094427_pos = 0;   /* 1 + index of p in support list, 0 if absent */
static size_t A094427_lim = 0;

static uint32_t *A094427_sup = 0;   /* primes with nonzero exponent */
static size_t A094427_supn = 0, A094427_supcap = 0;

static size_t A094427_sqcnt = 0;    /* number of primes with exponent >= 2 */
static uint64_t A094427_H = 0;      /* hash of the factorization */

static uint64_t *A094427_tab = 0;   /* open-addressing set of hashes seen */
static size_t A094427_tabcap = 0, A094427_tabn = 0;

static seq_t *A094427_ord = 0;
static size_t A094427_cnt = 0, A094427_cap = 0;
static idx_t A094427_it = 1;

static uint64_t A094427_mix(uint64_t p, uint64_t e)
{
    uint64_t z = (p << 32) ^ e ^ 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

static void A094427_ensure(size_t i)
{
    if (i < A094427_lim) return;
    size_t nl = A094427_lim ? A094427_lim * 2 : 1024;
    while (nl <= i) nl *= 2;
    A094427_spf = (uint32_t *)realloc(A094427_spf, nl * sizeof(uint32_t));
    A094427_exp = (uint32_t *)realloc(A094427_exp, nl * sizeof(uint32_t));
    A094427_pos = (uint32_t *)realloc(A094427_pos, nl * sizeof(uint32_t));
    for (size_t j = A094427_lim; j < nl; j++) {
        A094427_exp[j] = 0;
        A094427_pos[j] = 0;
    }
    for (size_t j = 0; j < nl; j++) A094427_spf[j] = 0;
    for (size_t j = 2; j < nl; j++) {
        if (A094427_spf[j] == 0) {
            for (size_t k = j; k < nl; k += j)
                if (A094427_spf[k] == 0) A094427_spf[k] = (uint32_t)j;
        }
    }
    A094427_lim = nl;
}

static void A094427_setexp(uint32_t p, uint32_t ne)
{
    uint32_t old = A094427_exp[p];
    if (old == ne) return;
    if (old) A094427_H ^= A094427_mix(p, old);
    if (ne) A094427_H ^= A094427_mix(p, ne);
    if (old >= 2) A094427_sqcnt--;
    if (ne >= 2) A094427_sqcnt++;
    if (old == 0) {
        if (A094427_supn == A094427_supcap) {
            A094427_supcap = A094427_supcap ? A094427_supcap * 2 : 64;
            A094427_sup = (uint32_t *)realloc(A094427_sup, A094427_supcap * sizeof(uint32_t));
        }
        A094427_sup[A094427_supn++] = p;
        A094427_pos[p] = (uint32_t)A094427_supn;
    } else if (ne == 0) {
        size_t k = A094427_pos[p] - 1;
        uint32_t last = A094427_sup[A094427_supn - 1];
        A094427_sup[k] = last;
        A094427_pos[last] = (uint32_t)(k + 1);
        A094427_supn--;
        A094427_pos[p] = 0;
    }
    A094427_exp[p] = ne;
}

/* true if current value < i */
static bool A094427_less_than(uint64_t i)
{
    uint64_t prod = 1;
    for (size_t k = 0; k < A094427_supn; k++) {
        uint32_t p = A094427_sup[k];
        uint32_t e = A094427_exp[p];
        for (uint32_t t = 0; t < e; t++) {
            prod *= p;
            if (prod >= i) return false;
        }
    }
    return prod < i;
}

/* insert hash; returns true if newly inserted */
static bool A094427_insert(uint64_t h)
{
    if (h == 0) h = 1;
    if (2 * (A094427_tabn + 1) > A094427_tabcap) {
        size_t nc = A094427_tabcap ? A094427_tabcap * 2 : 1024;
        uint64_t *nt = (uint64_t *)calloc(nc, sizeof(uint64_t));
        for (size_t j = 0; j < A094427_tabcap; j++) {
            uint64_t k = A094427_tab[j];
            if (k) {
                size_t s = (size_t)(k & (nc - 1));
                while (nt[s]) s = (s + 1) & (nc - 1);
                nt[s] = k;
            }
        }
        free(A094427_tab);
        A094427_tab = nt;
        A094427_tabcap = nc;
    }
    size_t s = (size_t)(h & (A094427_tabcap - 1));
    while (A094427_tab[s]) {
        if (A094427_tab[s] == h) return false;
        s = (s + 1) & (A094427_tabcap - 1);
    }
    A094427_tab[s] = h;
    A094427_tabn++;
    return true;
}

static seq_t A094427_value(void)
{
    seq_t v = (seq_t)1;
    for (size_t k = 0; k < A094427_supn; k++) {
        uint32_t p = A094427_sup[k];
        uint32_t e = A094427_exp[p];
        for (uint32_t t = 0; t < e; t++) v *= (seq_t)p;
    }
    return v;
}

seq_t a(idx_t n)
{
    while ((idx_t)A094427_cnt < n) {
        idx_t i = ++A094427_it;
        A094427_ensure((size_t)i);
        bool mult = A094427_less_than((uint64_t)i);
        uint32_t m = (uint32_t)i;
        while (m > 1) {
            uint32_t p = A094427_spf[m];
            uint32_t v = 0;
            while (m % p == 0) { m /= p; v++; }
            uint32_t e = A094427_exp[p];
            if (mult) A094427_setexp(p, e + v);
            else A094427_setexp(p, e > v ? e - v : 0);
        }
        if (A094427_sqcnt == 0) continue;
        if (!A094427_insert(A094427_H)) continue;
        if (A094427_cnt == A094427_cap) {
            size_t nc = A094427_cap ? A094427_cap * 2 : 256;
            seq_t *no = mem_new(seq_t, nc);
            for (size_t j = 0; j < A094427_cnt; j++) no[j] = A094427_ord[j];
            if (A094427_ord) mem_free(A094427_ord);
            A094427_ord = no;
            A094427_cap = nc;
        }
        A094427_ord[A094427_cnt++] = A094427_value();
    }
    return A094427_ord[n - 1];
}
