/*
 * A050014  Numbers k such that A050012(k) < A050012(k+1).
 * https://oeis.org/A050014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A050012(k) < A050012(k+1); found by scanning
 *            A050012 (a(1)=1, then floor(a(n-1)/2) if positive and unseen,
 *            else 7*a(n-1))
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050012_terms = NULL;
static size_t A050012_len = 0;
static size_t A050012_cap = 0;

static void A050012_ensure(size_t needed)
{
    if (needed <= A050012_cap)
        return;
    size_t new_cap = A050012_cap ? A050012_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A050012_terms, new_cap);
    if (!tmp)
        abort();
    A050012_terms = tmp;
    A050012_cap = new_cap;
}

static bool A050012_seen(seq_t v)
{
    for (size_t i = 0; i < A050012_len; ++i) {
        if (A050012_terms[i] == v)
            return true;
    }
    return false;
}

static seq_t A050012_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    size_t target = (size_t)n;
    A050012_ensure(target);
    if (A050012_len == 0) {
        A050012_terms[0] = (seq_t)1;
        A050012_len = 1;
    }
    while (A050012_len < target) {
        seq_t prev = A050012_terms[A050012_len - 1];
        seq_t cand = prev / (seq_t)2;
        if (cand > (seq_t)0 && !A050012_seen(cand))
            A050012_terms[A050012_len++] = cand;
        else
            A050012_terms[A050012_len++] = prev * (seq_t)7;
    }
    return A050012_terms[target - 1];
}

static idx_t *asc_list = NULL;
static size_t asc_len = 0;
static size_t asc_cap = 0;
static idx_t asc_next = 1;   /* next k to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    size_t target = (size_t)n;
    while (asc_len < target) {
        idx_t k = asc_next++;
        seq_t x = A050012_a(k);
        seq_t y = A050012_a(k + 1);
        if (x < y) {
            if (asc_len == asc_cap) {
                size_t nc = asc_cap ? asc_cap * 2 : 64;
                idx_t *t = (idx_t *)realloc(asc_list, nc * sizeof(idx_t));
                if (!t)
                    abort();
                asc_list = t;
                asc_cap = nc;
            }
            asc_list[asc_len++] = k;
        }
    }
    return (seq_t)asc_list[target - 1];
}
