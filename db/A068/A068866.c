/*
 * A068866  Triangular numbers k*(k+1)/2 such that A068865(k) = k*(k+1)/2.
 * https://oeis.org/A068866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search -- generate A068865 (a(1)=1; a(n)=smallest unused
 *            triangular T(k) differing from a(n-1) in every decimal digit);
 *            a(n) here = T(m) for the n-th index m with A068865(m) = T(m)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A068866_diff_all(seq_t x, seq_t y)
{
    while (x > 0 || y > 0) {
        unsigned dx = (unsigned)(long long)(x % 10);
        unsigned dy = (unsigned)(long long)(y % 10);
        if (dx == dy) return false;
        x /= 10;
        y /= 10;
    }
    return true;
}

static unsigned char *A068866_used = NULL;
static size_t A068866_used_size = 0;
static size_t A068866_low = 1;      /* smallest possibly-unused index */
static seq_t A068866_prev = 1;      /* previous term of A068865 */
static size_t A068866_cur = 1;      /* index of last generated term */
static bool A068866_init = false;

static seq_t *A068866_res = NULL;
static size_t A068866_cnt = 0;
static size_t A068866_cap = 0;

static void A068866_ensure(size_t k)
{
    if (k >= A068866_used_size) {
        size_t ns = A068866_used_size ? A068866_used_size * 2 : 1024;
        while (ns <= k) ns <<= 1;
        unsigned char *p = (unsigned char *)realloc(A068866_used, ns);
        if (!p) abort();
        memset(p + A068866_used_size, 0, ns - A068866_used_size);
        A068866_used = p;
        A068866_used_size = ns;
    }
}

static void A068866_push(seq_t v)
{
    if (A068866_cnt >= A068866_cap) {
        size_t nc = A068866_cap ? A068866_cap * 2 : 64;
        if (A068866_res == NULL) {
            A068866_res = mem_new(seq_t, nc);
        } else {
            A068866_res = mem_resize(seq_t, A068866_res, nc);
        }
        if (!A068866_res) abort();
        A068866_cap = nc;
    }
    A068866_res[A068866_cnt++] = v;
}

seq_t a(idx_t n)
{
    if (!A068866_init) {
        A068866_init = true;
        A068866_ensure(16);
        A068866_used[1] = 1;
        A068866_low = 2;
        A068866_prev = 1;
        A068866_cur = 1;
        A068866_push((seq_t)1);   /* A068865(1) = T(1) */
    }
    while (A068866_cnt < (size_t)n) {
        size_t k = A068866_low;
        seq_t val;
        for (;;) {
            A068866_ensure(k);
            if (A068866_used[k]) { k++; continue; }
            val = (seq_t)k * ((seq_t)k + 1) / 2;
            if (A068866_diff_all(A068866_prev, val)) break;
            k++;
        }
        A068866_used[k] = 1;
        A068866_cur++;
        A068866_prev = val;
        while (true) {
            A068866_ensure(A068866_low);
            if (A068866_used[A068866_low]) A068866_low++;
            else break;
        }
        if (k == A068866_cur) A068866_push(val);
    }
    return A068866_res[(size_t)n - 1];
}
