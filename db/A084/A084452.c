/*
 * A084452  An infinite juggling sequence with three balls: a(n) is zero when
 *          A084451(n-1) is even, otherwise the least (lowest) throw such that
 *          the resulting next juggling state A084451(n) does not occur in
 *          A084451[0..n-1].
 * https://oeis.org/A084452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/simulation: states s(0)=7; s(n)=s(n-1)/2 if even, else
 *            (s(n-1)-1)/2+2^e with least e such that s(n) is new with >=3
 *            one-bits (A084451); a(n)=0 if s(n-1) even, else e+1
 * Range:     u8 n <= 66, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t *A084452_st = NULL;
static size_t A084452_len = 0, A084452_cap = 0;
static size_t *A084452_tab = NULL;
static size_t A084452_tsz = 0;

static size_t A084452_hash(seq_t x)
{
    uint64_t lo = (uint64_t)(unsigned long long)(x & (seq_t)0xFFFFFFFFu);
    uint64_t hi = (uint64_t)(unsigned long long)((x >> 32) & (seq_t)0xFFFFFFFFu);
    uint64_t h = (hi << 32) | lo;
    h ^= h >> 33; h *= 0xff51afd7ed558ccdULL; h ^= h >> 33;
    return (size_t)h;
}

static void A084452_tab_insert(size_t idx)
{
    size_t m = A084452_tsz - 1;
    size_t p = A084452_hash(A084452_st[idx]) & m;
    while (A084452_tab[p]) p = (p + 1) & m;
    A084452_tab[p] = idx + 1;
}

static bool A084452_used(seq_t v)
{
    size_t m = A084452_tsz - 1;
    size_t p = A084452_hash(v) & m;
    while (A084452_tab[p]) {
        if (A084452_st[A084452_tab[p] - 1] == v) return true;
        p = (p + 1) & m;
    }
    return false;
}

static int A084452_pop3(seq_t x)
{
    int c = 0;
    while (x > 0 && c < 3) { if ((x & 1) != 0) c++; x >>= 1; }
    return c;
}

static void A084452_grow_table(void)
{
    size_t nt = A084452_tsz ? A084452_tsz * 2 : 1024;
    if (A084452_tab) free(A084452_tab);
    A084452_tab = (size_t *)calloc(nt, sizeof(size_t));
    if (!A084452_tab) exit(EXIT_FAILURE);
    A084452_tsz = nt;
    for (size_t i = 0; i < A084452_len; ++i) A084452_tab_insert(i);
}

static void A084452_ensure(size_t need)
{
    if (A084452_len == 0) {
        A084452_cap = 1024;
        A084452_st = mem_new(seq_t, A084452_cap);
        if (!A084452_st) exit(EXIT_FAILURE);
        A084452_st[0] = (seq_t)7;
        A084452_len = 1;
        A084452_grow_table();
    }
    if (need < A084452_len) return;
    if (need + 1 > A084452_cap) {
        size_t nc = A084452_cap * 2;
        if (nc < need + 1) nc = need + 1;
        seq_t *t = mem_resize(seq_t, A084452_st, nc);
        if (!t) exit(EXIT_FAILURE);
        A084452_st = t;
        A084452_cap = nc;
    }
    while (A084452_len <= need) {
        if ((A084452_len + 1) * 2 > A084452_tsz) A084452_grow_table();
        seq_t prev = A084452_st[A084452_len - 1];
        seq_t next;
        if ((prev & 1) == 0) {
            next = prev >> 1;
        } else {
            seq_t base = prev >> 1;
            idx_t e = 0;
            while (1) {
                next = base + pow2(e);
                if (!A084452_used(next) && A084452_pop3(next) >= 3) break;
                ++e;
            }
        }
        A084452_st[A084452_len] = next;
        A084452_tab_insert(A084452_len);
        ++A084452_len;
    }
}

seq_t a(idx_t n)
{
    A084452_ensure((size_t)n);
    seq_t s = A084452_st[(size_t)n - 1];
    if ((s & 1) == 0) return (seq_t)0;
    seq_t t = A084452_st[(size_t)n];
    seq_t d = t - (s >> 1);
    seq_t e = 0;
    while (d > 1) { d >>= 1; ++e; }
    return e + 1;
}
