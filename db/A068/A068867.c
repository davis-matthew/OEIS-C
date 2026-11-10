/*
 * A068867  Squares k^2 such that A068864(k) = k^2.
 * https://oeis.org/A068867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = k^2 where k is the n-th index with A068864(k) = k^2;
 *            found by a search over the greedy-generated A068864
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool A068864_digits_differ(seq_t x, seq_t y)
{
    while (x > 0 || y > 0) {
        unsigned dx = (unsigned)(x % 10);
        unsigned dy = (unsigned)(y % 10);
        if (dx == dy)
            return false;
        x /= 10;
        y /= 10;
    }
    return true;
}

static bool *A068864_used = NULL;
static size_t A068864_used_cap = 0;

static seq_t *A068864_seq = NULL;
static size_t A068864_seq_len = 0;
static size_t A068864_seq_cap = 0;

static void A068864_ensure_used_cap(size_t need)
{
    if (need + 1 <= A068864_used_cap)
        return;
    size_t new_cap = (A068864_used_cap == 0) ? 16 : A068864_used_cap;
    while (need + 1 > new_cap)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(A068864_used, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    for (size_t i = A068864_used_cap; i < new_cap; ++i)
        tmp[i] = false;
    A068864_used = tmp;
    A068864_used_cap = new_cap;
}

static void A068864_set_used(idx_t idx)
{
    if ((size_t)idx + 1 > A068864_used_cap)
        A068864_ensure_used_cap((size_t)idx);
    A068864_used[(size_t)idx] = true;
}

static bool A068864_is_used(idx_t idx)
{
    if ((size_t)idx + 1 > A068864_used_cap)
        return false;
    return A068864_used[(size_t)idx];
}

static void A068864_ensure_seq_cap(size_t need)
{
    if (need <= A068864_seq_cap)
        return;
    size_t new_cap = (A068864_seq_cap == 0) ? 64 : A068864_seq_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, A068864_seq, new_cap);
    if (!tmp)
        abort();
    A068864_seq = tmp;
    A068864_seq_cap = new_cap;
}

static seq_t A068864_a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if (A068864_seq_len == 0) {
        A068864_ensure_seq_cap(64);
        A068864_seq[A068864_seq_len++] = (seq_t)1;
        A068864_set_used((idx_t)1);
    }
    while (A068864_seq_len < (size_t)n) {
        seq_t prev = A068864_seq[A068864_seq_len - 1];
        for (idx_t m = 1;; ++m) {
            if (A068864_is_used(m))
                continue;
            seq_t cand = (seq_t)m;
            cand *= cand;
            if (!A068864_digits_differ(prev, cand))
                continue;
            A068864_set_used(m);
            A068864_ensure_seq_cap(A068864_seq_len + 1);
            A068864_seq[A068864_seq_len++] = cand;
            break;
        }
    }
    return A068864_seq[n - 1];
}

static idx_t *found_k = NULL;
static size_t found_len = 0;
static size_t found_cap = 0;
static idx_t scan_k = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    while (found_len < (size_t)n) {
        scan_k++;
        seq_t v = A068864_a(scan_k);
        seq_t sq = (seq_t)scan_k * (seq_t)scan_k;
        if (v == sq) {
            if (found_len == found_cap) {
                size_t nc = found_cap ? found_cap * 2 : 64;
                idx_t *t = (idx_t *)realloc(found_k, nc * sizeof(idx_t));
                if (!t)
                    abort();
                found_k = t;
                found_cap = nc;
            }
            found_k[found_len++] = scan_k;
        }
    }
    idx_t k = found_k[(size_t)(n - 1)];
    return (seq_t)k * (seq_t)k;
}
