/*
 * A050125  a(n)=position of n in A050124.
 * https://oeis.org/A050125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050124, where A050124(1)=1 and
 *            A050124(k) = floor(A050124(k-1)/5) if that is not 0 or an
 *            earlier term, otherwise 2*A050124(k-1). Computed by generating
 *            A050124 with a hash set until n appears
 * Range:     u8 n <= 10, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A050125_HB 1048573u
#define A050125_LIM 200000u

static seq_t *A050125_s = NULL;
static uint32_t *A050125_nx = NULL;
static uint32_t *A050125_hd = NULL;
static unsigned long long *A050125_pos = NULL;
static size_t A050125_len = 0, A050125_cap = 0;

static void A050125_insert(seq_t v)
{
    size_t idx = A050125_len;
    if (idx + 1 > A050125_cap) {
        size_t nc = A050125_cap ? A050125_cap * 2 : 1024;
        A050125_s = mem_resize(seq_t, A050125_s, nc);
        A050125_nx = (uint32_t *)realloc(A050125_nx, nc * sizeof(uint32_t));
        A050125_cap = nc;
    }
    A050125_s[idx] = v;
    size_t b = (size_t)(v % (seq_t)A050125_HB);
    A050125_nx[idx] = A050125_hd[b];
    A050125_hd[b] = (uint32_t)(idx + 1);
    A050125_len = idx + 1;
    if (v <= (seq_t)A050125_LIM) {
        size_t k = (size_t)v;
        if (A050125_pos[k] == 0) A050125_pos[k] = (unsigned long long)A050125_len;
    }
}

static bool A050125_has(seq_t v)
{
    size_t b = (size_t)(v % (seq_t)A050125_HB);
    uint32_t i = A050125_hd[b];
    while (i) {
        if (A050125_s[i - 1] == v) return true;
        i = A050125_nx[i - 1];
    }
    return false;
}

static void A050125_step(void)
{
    seq_t prev = A050125_s[A050125_len - 1];
    seq_t cand = prev / 5;
    if (cand == 0 || A050125_has(cand))
        A050125_insert(prev * 2);
    else
        A050125_insert(cand);
}

seq_t a(idx_t n)
{
    if (!A050125_hd) {
        A050125_hd = (uint32_t *)calloc(A050125_HB, sizeof(uint32_t));
        A050125_pos = (unsigned long long *)calloc((size_t)A050125_LIM + 1, sizeof(unsigned long long));
        A050125_insert((seq_t)1);
    }
    if (n == 0) return (seq_t)0;
    if (n <= (idx_t)A050125_LIM) {
        while (A050125_pos[(size_t)n] == 0) A050125_step();
        return (seq_t)A050125_pos[(size_t)n];
    }
    seq_t target = (seq_t)n;
    size_t i = 0;
    for (;;) {
        for (; i < A050125_len; ++i)
            if (A050125_s[i] == target) return (seq_t)(i + 1);
        A050125_step();
    }
}
