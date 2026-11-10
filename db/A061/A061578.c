/*
 * A061578  First differences of A061577.
 * https://oeis.org/A061578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = A061577(n+1) - A061577(n); A061577 is built greedily
 *            (a(1)=2, a(2)=3, then the smallest x > a(n-1) with x and
 *            x-a(n-1) both unused)
 * Range:     u8 n <= 20, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A061577_seq      = NULL;
static idx_t  A061577_seq_len  = 0;
static idx_t  A061577_seq_cap  = 0;

static uint8_t *A061577_used      = NULL;
static idx_t   A061577_used_cap   = 0;

static void A061577_ensure_used(idx_t v)
{
    if (v < A061577_used_cap) return;
    idx_t new_cap = A061577_used_cap ? A061577_used_cap * 2 : 64;
    while (new_cap <= v) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(A061577_used, (size_t)new_cap);
    if (!tmp) abort();
    for (idx_t i = A061577_used_cap; i < new_cap; ++i) tmp[i] = 0;
    A061577_used = tmp;
    A061577_used_cap = new_cap;
}

static void A061577_init_sequence(void)
{
    if (A061577_seq) return;

    A061577_seq_cap = 4;
    A061577_seq = mem_new(seq_t, A061577_seq_cap);
    if (!A061577_seq) abort();
    A061577_seq[0] = (seq_t)2;
    A061577_seq[1] = (seq_t)3;
    A061577_seq_len = 2;

    A061577_used_cap = 64;
    A061577_used = (uint8_t*)calloc((size_t)A061577_used_cap, 1);
    if (!A061577_used) abort();

    A061577_used[1] = 1;
    A061577_used[2] = 1;
    A061577_used[3] = 1;
}

static void A061577_generate_upto(idx_t n)
{
    A061577_init_sequence();
    while (A061577_seq_len < n) {
        idx_t last = (idx_t)A061577_seq[A061577_seq_len - 1];
        idx_t candidate = last + 1;

        while (true) {
            A061577_ensure_used(candidate);
            idx_t diff = candidate - last;
            A061577_ensure_used(diff);
            if (!A061577_used[candidate] && !A061577_used[diff]) break;
            ++candidate;
        }

        if (A061577_seq_len == A061577_seq_cap) {
            A061577_seq_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, A061577_seq, A061577_seq_cap);
            if (!tmp) abort();
            A061577_seq = tmp;
        }
        A061577_seq[A061577_seq_len] = (seq_t)candidate;
        ++A061577_seq_len;

        A061577_used[candidate] = 1;
        A061577_used[candidate - last] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A061577_generate_upto(n + 1);
    return A061577_seq[n] - A061577_seq[n - 1];
}
