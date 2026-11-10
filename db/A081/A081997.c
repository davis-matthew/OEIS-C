/*
 * A081997  Row sums in triangle A081994.
 * https://oeis.org/A081997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} T(n,k), T = A081994; computed by building
 *            the triangle greedily (smallest unused number coprime to
 *            everything to its left in the row and above it in the column),
 *            using a smallest-prime-factor sieve and prime marks so each
 *            candidate is checked in native integers
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint64_t *A081997_tri = NULL;
static size_t A081997_tri_cap = 0;
static seq_t *A081997_sums = NULL;
static size_t A081997_sums_cap = 0;
static size_t A081997_rows = 0;
static size_t A081997_first_free = 1;

static size_t A081997_lim = 0;          /* arrays below are valid for indices < lim */
static uint32_t *A081997_spf = NULL;
static uint32_t *A081997_mark = NULL;
static unsigned char *A081997_used = NULL;
static uint32_t A081997_stamp = 0;

static void A081997_grow(size_t need)
{
    if (need < A081997_lim) return;
    size_t nl = A081997_lim ? A081997_lim : 1024;
    while (nl <= need) nl *= 2;
    A081997_spf = (uint32_t *)realloc(A081997_spf, nl * sizeof(uint32_t));
    A081997_mark = (uint32_t *)realloc(A081997_mark, nl * sizeof(uint32_t));
    A081997_used = (unsigned char *)realloc(A081997_used, nl);
    memset(A081997_mark + A081997_lim, 0, (nl - A081997_lim) * sizeof(uint32_t));
    memset(A081997_used + A081997_lim, 0, nl - A081997_lim);
    memset(A081997_spf, 0, nl * sizeof(uint32_t));
    for (size_t i = 2; i < nl; ++i) {
        if (A081997_spf[i] == 0) {
            A081997_spf[i] = (uint32_t)i;
            if (i <= (nl - 1) / i)
                for (size_t j = i * i; j < nl; j += i)
                    if (A081997_spf[j] == 0) A081997_spf[j] = (uint32_t)i;
        }
    }
    A081997_lim = nl;
}

static void A081997_mark_value(uint64_t x)
{
    while (x > 1) {
        uint32_t p = A081997_spf[x];
        A081997_mark[p] = A081997_stamp;
        while (x % p == 0) x /= p;
    }
}

static bool A081997_ok(uint64_t x)
{
    while (x > 1) {
        uint32_t p = A081997_spf[x];
        if (A081997_mark[p] == A081997_stamp) return false;
        while (x % p == 0) x /= p;
    }
    return true;
}

static void A081997_build(size_t rows)
{
    A081997_grow(1);
    while (A081997_rows < rows) {
        size_t r = A081997_rows + 1;
        size_t off = r * (r - 1) / 2;
        size_t need = off + r;
        if (A081997_tri_cap < need) {
            size_t nc = A081997_tri_cap ? A081997_tri_cap : 64;
            while (nc < need) nc *= 2;
            A081997_tri = (uint64_t *)realloc(A081997_tri, nc * sizeof(uint64_t));
            A081997_tri_cap = nc;
        }
        if (A081997_sums_cap < r) {
            size_t nc = A081997_sums_cap ? A081997_sums_cap * 2 : 16;
            while (nc < r) nc *= 2;
            A081997_sums = mem_resize(seq_t, A081997_sums, nc);
            A081997_sums_cap = nc;
        }
        seq_t sum = 0;
        for (size_t c = 1; c <= r; ++c) {
            ++A081997_stamp;
            for (size_t j = 1; j < c; ++j)
                A081997_mark_value(A081997_tri[off + j - 1]);
            for (size_t i = c; i < r; ++i)
                A081997_mark_value(A081997_tri[i * (i - 1) / 2 + c - 1]);
            size_t cand = A081997_first_free;
            for (;; ++cand) {
                A081997_grow(cand);
                if (A081997_used[cand]) continue;
                if (A081997_ok((uint64_t)cand)) break;
            }
            A081997_used[cand] = 1;
            A081997_tri[off + c - 1] = (uint64_t)cand;
            sum += (seq_t)(uint64_t)cand;
            for (;;) {
                A081997_grow(A081997_first_free);
                if (!A081997_used[A081997_first_free]) break;
                ++A081997_first_free;
            }
        }
        A081997_sums[r - 1] = sum;
        A081997_rows = r;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A081997_build((size_t)n);
    return A081997_sums[n - 1];
}
