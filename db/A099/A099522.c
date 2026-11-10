/*
 * A099522  RECORDS transform of A099520.
 * https://oeis.org/A099522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   RECORDS transform of A099520 (a(1)=1; a(n)=smallest unused m>0
 *            with m+a(n-1) divisible by n-1); computed by simulation, listing
 *            the left-to-right maxima
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static bool  *r_used = NULL;
static size_t r_used_cap = 0;
static seq_t *r_rec = NULL;
static size_t r_rec_len = 0, r_rec_cap = 0;
static idx_t  r_i = 0;
static seq_t  r_prev = 0, r_max = 0;

static void r_ensure_used(size_t n)
{
    if (n <= r_used_cap) return;
    size_t cap = r_used_cap ? r_used_cap : 64;
    while (cap < n) cap <<= 1;
    bool *t = (bool *)realloc(r_used, cap * sizeof(bool));
    if (!t) abort();
    memset(t + r_used_cap, 0, (cap - r_used_cap) * sizeof(bool));
    r_used = t;
    r_used_cap = cap;
}

static void r_push(seq_t v)
{
    if (r_rec_len >= r_rec_cap) {
        size_t cap = r_rec_cap ? r_rec_cap << 1 : 64;
        seq_t *t = mem_resize(seq_t, r_rec, cap);
        if (!t) abort();
        r_rec = t;
        r_rec_cap = cap;
    }
    r_rec[r_rec_len++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (r_i == 0) {
        r_ensure_used(64);
        r_used[1] = true;
        r_i = 1;
        r_prev = 1;
        r_max = 1;
        r_push((seq_t)1);
    }
    while (r_rec_len < (size_t)n) {
        r_i++;
        seq_t mod = (seq_t)(r_i - 1);
        seq_t rem = r_prev % mod;
        seq_t cand = (rem == 0) ? mod : mod - rem;
        for (;;) {
            r_ensure_used((size_t)(long long)cand + 1);
            if (!r_used[(size_t)(long long)cand]) break;
            cand += mod;
        }
        r_used[(size_t)(long long)cand] = true;
        r_prev = cand;
        if (cand > r_max) {
            r_max = cand;
            r_push(cand);
        }
    }
    return r_rec[(size_t)(n - 1)];
}
