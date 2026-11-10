/*
 * A091840  Lengths of suffix blocks associated with A091787.
 * https://oeis.org/A091840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. A091787 = B_oo with B_1 = 2 and B_{m+1} = B_m^3 S_{m+1}.
 *            * Each suffix block S_{m+1} contains only terms >= 3. The suffix
 *            blocks sit * next to each other at the end of B_m, so their
 *            concatenation W = S_2 S_3 ... * follows its own curling rule: *
 *            - start a new block by appending max(3, cur...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static int    *A091840_W = NULL;
static size_t  A091840_wlen = 0, A091840_wcap = 0;
static size_t *A091840_lens = NULL;
static size_t  A091840_nl = 0, A091840_lcap = 0;

static int A091840_push(int v)
{
    if (A091840_wlen == A091840_wcap) {
        size_t nc = A091840_wcap ? 2 * A091840_wcap : 1024;
        int *p = (int *)realloc(A091840_W, nc * sizeof(int));
        if (!p) return 0;
        A091840_W = p;
        A091840_wcap = nc;
    }
    A091840_W[A091840_wlen++] = v;
    return 1;
}

/* curling number of W (largest k with W = x y^k, |y| >= 1) */
static size_t A091840_curl(void)
{
    const int *w = A091840_W;
    size_t len = A091840_wlen;
    size_t maxk = 1;
    if (len == 0) return 0;
    for (size_t L = 1; L <= len / 2 && len / L > maxk; ++L) {
        size_t pos = len - L;
        size_t k = 1;
        while (pos >= L) {
            size_t j;
            for (j = 0; j < L; ++j)
                if (w[pos - L + j] != w[pos + j]) break;
            if (j < L) break;
            ++k;
            pos -= L;
        }
        if (k > maxk) maxk = k;
    }
    return maxk;
}

static int A091840_gen_block(void)
{
    size_t start = A091840_wlen;
    size_t c = A091840_curl();
    if (!A091840_push((int)(c < 3 ? 3 : c))) return 0;
    for (;;) {
        c = A091840_curl();
        if (c < 3) break;
        if (!A091840_push((int)c)) return 0;
    }
    if (A091840_nl == A091840_lcap) {
        size_t nc = A091840_lcap ? 2 * A091840_lcap : 128;
        size_t *p = (size_t *)realloc(A091840_lens, nc * sizeof(size_t));
        if (!p) return 0;
        A091840_lens = p;
        A091840_lcap = nc;
    }
    A091840_lens[A091840_nl++] = A091840_wlen - start;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while ((idx_t)A091840_nl < n) {
        if (!A091840_gen_block()) return 0;
    }
    return (seq_t)(uint64_t)A091840_lens[n - 1];
}
