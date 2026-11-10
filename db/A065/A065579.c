/*
 * A065579  Inverse permutation to A065562.
 * https://oeis.org/A065579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that A065562(k) = n; search: simulate A065562
 *            (b(k)-th smallest unused integer, b = A002260) until n appears
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9697 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static unsigned char *A065579_used = NULL;
static idx_t *A065579_pos = NULL;
static size_t A065579_cap = 0;
static size_t A065579_next_free = 1;
static idx_t A065579_k = 0;   /* terms generated so far */
static idx_t A065579_r = 1;   /* current row */
static idx_t A065579_c = 0;   /* current column (b value) */

static void A065579_grow(size_t needed)
{
    if (needed < A065579_cap) return;
    size_t nc = A065579_cap ? A065579_cap : 1024;
    while (nc <= needed) nc *= 2;
    unsigned char *u = (unsigned char *)realloc(A065579_used, nc);
    idx_t *p = (idx_t *)realloc(A065579_pos, nc * sizeof(idx_t));
    if (!u || !p) exit(1);
    for (size_t i = A065579_cap; i < nc; ++i) { u[i] = 0; p[i] = 0; }
    A065579_used = u;
    A065579_pos = p;
    A065579_cap = nc;
}

static void A065579_step(void)
{
    ++A065579_k;
    ++A065579_c;
    if (A065579_c > A065579_r) { ++A065579_r; A065579_c = 1; }
    idx_t b = A065579_c;
    idx_t count = 0;
    size_t cand = A065579_next_free;
    for (;;) {
        A065579_grow(cand + 1);
        if (!A065579_used[cand]) {
            ++count;
            if (count == b) break;
        }
        ++cand;
    }
    A065579_used[cand] = 1;
    A065579_pos[cand] = A065579_k;
    while (1) {
        A065579_grow(A065579_next_free + 1);
        if (!A065579_used[A065579_next_free]) break;
        ++A065579_next_free;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A065579_grow((size_t)n + 2);
    while (A065579_pos[(size_t)n] == 0) {
        A065579_step();
        A065579_grow((size_t)n + 2);
    }
    return (seq_t)A065579_pos[(size_t)n];
}
