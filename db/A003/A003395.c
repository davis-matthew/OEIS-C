/*
 * A003395  Numbers that are the sum of 6 positive 9th powers.
 * https://oeis.org/A003395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: enumerate all sums of 6 positive 9th powers up to a
 *            bound L (nondecreasing bases), sort, remove duplicates; enlarge
 *            L until n terms exist
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 6756, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3395_cmp64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A3395_cmp64.h"

static uint64_t *A3395_T = NULL;
static size_t A3395_n = 0, A3395_cap = 0;
static uint64_t A3395_P[130];
static int A3395_np = 0;
static uint64_t A3395_L = 0;

static void A3395_push(uint64_t v)
{
    if (A3395_n >= A3395_cap) {
        size_t nc = A3395_cap ? A3395_cap * 2 : 4096;
        A3395_T = (uint64_t *)realloc(A3395_T, nc * sizeof(uint64_t));
        A3395_cap = nc;
    }
    A3395_T[A3395_n++] = v;
}

static void A3395_gen(int k, int minb, uint64_t sum, uint64_t L)
{
    int b;
    if (k == 0) { A3395_push(sum); return; }
    for (b = minb; b <= A3395_np; b++) {
        if (A3395_P[b] > (L - sum) / (uint64_t)k) break;
        A3395_gen(k - 1, b, sum + A3395_P[b], L);
    }
}

static void A3395_build(uint64_t L)
{
    int b, j;
    size_t i, w;
    A3395_np = 0;
    for (b = 1; b <= 128; b++) {
        uint64_t p = 1;
        for (j = 0; j < 9; j++) p *= (uint64_t)b;
        A3395_P[b] = p;
        if (p <= L) A3395_np = b;
    }
    A3395_n = 0;
    A3395_gen(6, 1, 0, L);
    qsort(A3395_T, A3395_n, sizeof(uint64_t), A3395_cmp64);
    w = 0;
    for (i = 0; i < A3395_n; i++)
        if (w == 0 || A3395_T[i] != A3395_T[w - 1]) A3395_T[w++] = A3395_T[i];
    A3395_n = w;
    A3395_L = L;
}

seq_t a(idx_t n)
{
    if (A3395_L == 0) A3395_build((uint64_t)1 << 16);
    while ((uint64_t)A3395_n < n && A3395_L < ((uint64_t)1 << 62)) {
        uint64_t L = A3395_L * 8;
        if (L > ((uint64_t)1 << 62)) L = (uint64_t)1 << 62;
        A3395_build(L);
    }
    return (seq_t)A3395_T[n - 1];
}
