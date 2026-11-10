/*
 * A085328  Record values in A085068.
 * https://oeis.org/A085328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record value of A085068, where A085068(m) = 1 +
 *            min{i >= 0 : 3 | c_i}, c_0 = m, c_{i+1} = ceiling(4*c_i/3).
 *            Computed by a search: the start values m are scanned in doubling
 *            ranges with a 3-adic DFS. Adding t*3^j to m adds t*4^j to c_j,
 *            so the residues of m mod 3^j that survive j steps...
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A085328_u128;

typedef struct { uint64_t n; int v; } A085328_cand;

static A085328_cand *A085328_cands = 0;
static size_t A085328_ccount = 0, A085328_ccap = 0;
static int A085328_curR = 0;
static uint64_t A085328_lo = 0, A085328_hi = 2;

static int *A085328_recs = 0;
static size_t A085328_rcount = 0, A085328_rcap = 0;

static void A085328_add_cand(uint64_t n, int v)
{
    if (A085328_ccount == A085328_ccap) {
        size_t nc = A085328_ccap ? A085328_ccap * 2 : 256;
        A085328_cands = (A085328_cand *)realloc(A085328_cands, nc * sizeof(A085328_cand));
        A085328_ccap = nc;
    }
    A085328_cands[A085328_ccount].n = n;
    A085328_cands[A085328_ccount].v = v;
    A085328_ccount++;
}

static void A085328_add_rec(int v)
{
    if (A085328_rcount == A085328_rcap) {
        size_t nc = A085328_rcap ? A085328_rcap * 2 : 64;
        A085328_recs = (int *)realloc(A085328_recs, nc * sizeof(int));
        A085328_rcap = nc;
    }
    A085328_recs[A085328_rcount++] = v;
}

/* r: residue mod pw = 3^j; c = c_j(r); c_0..c_{j-1} not divisible by 3; p4 = 4^j */
static void A085328_dfs(uint64_t r, int j, A085328_u128 c, A085328_u128 pw, A085328_u128 p4)
{
    const uint64_t lo = A085328_lo, hi = A085328_hi;
    if ((A085328_u128)r + pw >= hi) {
        if (r < lo) return;
        int i = j;
        while (c % 3 != 0 && i < 4000) {
            if (c > ((A085328_u128)1 << 124)) break;
            c = (4 * c + 2) / 3;
            i++;
        }
        int v = i + 1;
        if (v > A085328_curR) A085328_add_cand(r, v);
        return;
    }
    A085328_u128 step = 3 * pw;
    for (int t = 0; t < 3; t++) {
        A085328_u128 rr = (A085328_u128)r + (A085328_u128)t * pw;
        if (rr >= hi) break;
        A085328_u128 cc = c + (A085328_u128)t * p4;
        A085328_u128 first = rr;
        if (first < lo) first += (((A085328_u128)lo - first + step - 1) / step) * step;
        if (first >= hi) continue;
        if (cc % 3 == 0) {
            if (j + 1 > A085328_curR) A085328_add_cand((uint64_t)first, j + 1);
            continue;
        }
        A085328_dfs((uint64_t)rr, j + 1, (4 * cc + 2) / 3, step, 4 * p4);
    }
}

static int A085328_cmp(const void *x, const void *y)
{
    const A085328_cand *p = (const A085328_cand *)x;
    const A085328_cand *q = (const A085328_cand *)y;
    if (p->n < q->n) return -1;
    if (p->n > q->n) return 1;
    return 0;
}

static void A085328_process_range(void)
{
    A085328_ccount = 0;
    A085328_dfs(0, 0, 0, 1, 1);
    if (A085328_ccount > 1)
        qsort(A085328_cands, A085328_ccount, sizeof(A085328_cand), A085328_cmp);
    for (size_t i = 0; i < A085328_ccount; i++) {
        if (A085328_cands[i].v > A085328_curR) {
            A085328_curR = A085328_cands[i].v;
            A085328_add_rec(A085328_curR);
        }
    }
    A085328_lo = A085328_hi;
    A085328_hi = A085328_hi * 2;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while ((idx_t)A085328_rcount < n) {
        if (A085328_hi >= ((uint64_t)1 << 62)) break;
        A085328_process_range();
    }
    if ((idx_t)A085328_rcount < n) return (seq_t)0;
    return (seq_t)A085328_recs[(size_t)(n - 1)];
}
