/*
 * A054085  a(n) = order of n in the permutation A054084 of the natural
 *          numbers if this order exists; a(n) = -1 otherwise.
 * https://oeis.org/A054085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = length of the cycle of n under the permutation A054084
 *            (-1 if the orbit does not close); computed by iterating the
 *            permutation, with a size cap to declare an infinite orbit
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t *p54_perm = NULL;
static size_t    p54_len = 0, p54_cap = 0;
static unsigned char *p54_used = NULL;
static size_t    p54_ucap = 0;
static size_t    p54_cand = 1;

static void p54_ensure_used(size_t i)
{
    if (i < p54_ucap) return;
    size_t nc = p54_ucap ? p54_ucap : 1024;
    while (nc <= i) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(p54_used, nc);
    if (!t) abort();
    for (size_t j = p54_ucap; j < nc; ++j) t[j] = 0;
    p54_used = t;
    p54_ucap = nc;
}

static void p54_grow_perm(size_t need)
{
    if (need <= p54_cap) return;
    size_t nc = p54_cap ? p54_cap : 1024;
    while (nc < need) nc <<= 1;
    uint32_t *t = (uint32_t *)realloc(p54_perm, nc * sizeof(uint32_t));
    if (!t) abort();
    p54_perm = t;
    p54_cap = nc;
}

static void p54_fill(size_t pos)
{
    while (p54_len < pos) {
        size_t k = p54_len / 2 + 1;
        p54_ensure_used(p54_cand);
        while (p54_used[p54_cand]) {
            ++p54_cand;
            p54_ensure_used(p54_cand);
        }
        size_t p = p54_cand;
        size_t q = p + k;
        p54_ensure_used(q);
        p54_used[p] = 1;
        p54_used[q] = 1;
        p54_grow_perm(p54_len + 2);
        p54_perm[p54_len++] = (uint32_t)q;
        p54_perm[p54_len++] = (uint32_t)p;
        ++p54_cand;
    }
}

seq_t a(idx_t n)
{
    size_t start = (size_t)n;
    size_t lim = 2000000;
    if (start > lim) lim = start;
    p54_fill(start + 2);
    size_t cur = p54_perm[start - 1];
    size_t steps = 1;
    while (cur != start) {
        if (cur > lim || steps > lim) return (seq_t)(-1);
        p54_fill(cur + 2);
        cur = p54_perm[cur - 1];
        ++steps;
    }
    return (seq_t)steps;
}
