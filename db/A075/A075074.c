/*
 * A075074  Integers pertaining to A099520: a(n) = {b(n)+b(n+1)}/n where b(n)
 *          is the n-th term of A099520.
 * https://oeis.org/A075074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n)+b(n+1))/n, where b = A099520: b(1)=1; for n>1,
 *            b(n)=smallest m>0 not yet used with m+b(n-1) divisible by n-1
 *            (computed by direct search)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A075074_b = NULL;
static size_t A075074_len = 0;
static size_t A075074_cap = 0;
static bool *A075074_used = NULL;
static size_t A075074_ucap = 0;

static void A075074_ensure_b(size_t idx)
{
    if (idx < A075074_cap) return;
    size_t cap = A075074_cap ? A075074_cap << 1 : 16;
    while (cap <= idx) cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A075074_b, cap);
    if (!tmp) abort();
    A075074_b = tmp;
    A075074_cap = cap;
}

static void A075074_ensure_used(size_t n)
{
    if (n <= A075074_ucap) return;
    size_t cap = A075074_ucap ? A075074_ucap << 1 : 16;
    while (cap < n) cap <<= 1;
    bool *tmp = (bool *)realloc(A075074_used, cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + A075074_ucap, 0, (cap - A075074_ucap) * sizeof(bool));
    A075074_used = tmp;
    A075074_ucap = cap;
}

static seq_t A075074_term(idx_t n)
{
    if (A075074_len == 0) {
        A075074_ensure_b(1);
        A075074_b[1] = (seq_t)1;
        A075074_len = 1;
        A075074_ensure_used(2);
        A075074_used[1] = true;
    }
    for (idx_t i = (idx_t)A075074_len + 1; i <= n; ++i) {
        seq_t mod = (seq_t)(i - 1);
        seq_t prev = A075074_b[(size_t)(i - 1)];
        seq_t rem = prev % mod;
        seq_t cand = (rem == 0) ? mod : mod - rem;
        while (true) {
            A075074_ensure_used((size_t)cand + 1);
            if (!A075074_used[(size_t)cand]) break;
            cand += mod;
        }
        A075074_ensure_b((size_t)i);
        A075074_b[(size_t)i] = cand;
        A075074_len = (size_t)i;
        A075074_used[(size_t)cand] = true;
    }
    return A075074_b[(size_t)n];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t x = A075074_term(n + 1);
    seq_t y = A075074_b[(size_t)n];
    return (y + x) / (seq_t)n;
}
