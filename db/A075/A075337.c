/*
 * A075337  Products of members of pairs in A075333.
 * https://oeis.org/A075337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n)*s(n), where (r,s) is the n-th pair of A075333
 *            (greedy search)
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 151, u64 n <= 151,
 *            u128 n <= 151, big any n
 * Validated: standard, pass, 56 terms through n = 147 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A075337_prod = NULL;
static size_t A075337_cap = 0;
static size_t A075337_len = 1;

static seq_t *A075337_used = NULL;
static size_t A075337_ucap = 0;
static size_t A075337_ulen = 0;

static seq_t A075337_next_r = 2;

static bool A075337_isused(seq_t v)
{
    for (size_t i = 0; i < A075337_ulen; ++i)
        if (A075337_used[i] == v) return true;
    return false;
}

seq_t a(idx_t n)
{
    while (A075337_len <= (size_t)n) {
        seq_t r = A075337_next_r;
        seq_t s = r + 1;
        seq_t sum, diff, prod;
        for (;;) {
            sum = r + s;
            diff = s - r;
            prod = r * s;
            if (r == s || r == sum || r == diff || r == prod ||
                s == sum || s == diff || s == prod ||
                sum == diff || sum == prod || diff == prod) {
                ++s;
                continue;
            }
            if (A075337_isused(r) || A075337_isused(s) || A075337_isused(sum) ||
                A075337_isused(diff) || A075337_isused(prod)) {
                ++s;
                continue;
            }
            break;
        }
        if (A075337_ulen + 5 > A075337_ucap) {
            size_t nc = A075337_ucap ? A075337_ucap * 2 : 64;
            while (nc < A075337_ulen + 5) nc <<= 1;
            A075337_used = mem_resize(seq_t, A075337_used, nc);
            A075337_ucap = nc;
        }
        A075337_used[A075337_ulen++] = r;
        A075337_used[A075337_ulen++] = s;
        A075337_used[A075337_ulen++] = sum;
        A075337_used[A075337_ulen++] = diff;
        A075337_used[A075337_ulen++] = prod;

        if (A075337_len + 1 > A075337_cap) {
            size_t nc = A075337_cap ? A075337_cap * 2 : 64;
            while (nc < A075337_len + 1) nc <<= 1;
            A075337_prod = mem_resize(seq_t, A075337_prod, nc);
            A075337_cap = nc;
        }
        A075337_prod[A075337_len++] = prod;

        seq_t cand = r + 1;
        while (A075337_isused(cand)) ++cand;
        A075337_next_r = cand;
    }
    return A075337_prod[(size_t)n];
}
