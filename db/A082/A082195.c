/*
 * A082195  Sum of n-th antidiagonal of A082191.
 * https://oeis.org/A082195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=n(n-1)/2+1..n(n+1)/2} A082191(k); computed by
 *            simulating the A082191 array fill (search)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

#define A082195_DIM 512

static seq_t *A082195_grid = NULL;
static seq_t *A082195_vals = NULL;
static size_t A082195_cap = 0;
static size_t A082195_len = 0;
static char *A082195_used = NULL;
static size_t A082195_usedSize = 0;
static unsigned A082195_s = 3, A082195_pos = 0;

static void A082195_ensure_used(size_t needed)
{
    if (A082195_usedSize > needed) return;
    size_t ns = A082195_usedSize ? A082195_usedSize * 2 : 1024;
    while (ns <= needed) ns *= 2;
    char *p = (char *)realloc(A082195_used, ns);
    if (!p) abort();
    memset(p + A082195_usedSize, 0, ns - A082195_usedSize);
    A082195_used = p;
    A082195_usedSize = ns;
}

static void A082195_push(seq_t v)
{
    if (A082195_len >= A082195_cap) {
        size_t nc = A082195_cap ? A082195_cap * 2 : 256;
        seq_t *p = mem_resize(seq_t, A082195_vals, nc);
        if (!p) abort();
        A082195_vals = p;
        A082195_cap = nc;
    }
    A082195_vals[A082195_len++] = v;
}

static void A082195_init(void)
{
    if (A082195_grid) return;
    A082195_grid = mem_new(seq_t, (size_t)A082195_DIM * A082195_DIM);
    if (!A082195_grid) abort();
    A082195_ensure_used(1024);
    A082195_used[1] = 1;
    A082195_grid[0] = 1;
    A082195_push((seq_t)1);
}

static void A082195_gen_one(void)
{
    unsigned s = A082195_s;
    unsigned i, j;
    if (s % 2 == 1) i = 1 + A082195_pos;
    else i = (s - 1) - A082195_pos;
    j = s - i;
    A082195_pos++;
    if (A082195_pos == s - 1) { A082195_s++; A082195_pos = 0; }

    seq_t cand = 2;
    for (;;) {
        size_t c = (size_t)cand;
        A082195_ensure_used(c + 1);
        if (A082195_used[c]) { cand++; continue; }
        bool ok = true;
        for (int dx = -1; dx <= 1 && ok; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int ni = (int)i + dx, nj = (int)j + dy;
                if (ni < 1 || ni > A082195_DIM || nj < 1 || nj > A082195_DIM) continue;
                seq_t nb = A082195_grid[(size_t)(ni - 1) * A082195_DIM + (nj - 1)];
                if (nb > (seq_t)1) {
                    if (gcd(cand, nb) <= (seq_t)1) { ok = false; break; }
                }
            }
        }
        if (ok) break;
        cand++;
    }
    A082195_grid[(size_t)(i - 1) * A082195_DIM + (j - 1)] = cand;
    A082195_used[(size_t)cand] = 1;
    A082195_push(cand);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A082195_init();
    size_t hi = (size_t)n * ((size_t)n + 1) / 2;
    size_t lo = (size_t)n * ((size_t)n - 1) / 2;
    while (A082195_len < hi) A082195_gen_one();
    seq_t sum = 0;
    for (size_t k = lo; k < hi; ++k) sum += A082195_vals[k];
    return sum;
}
