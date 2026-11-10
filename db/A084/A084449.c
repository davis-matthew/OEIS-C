/*
 * A084449  Positions of sevens (ground states) in A084451.
 * https://oeis.org/A084449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search. Generate A084451 by its recurrence: b(0) = 7; b(n) =
 *            b(n-1)/2 if b(n-1) is even, otherwise b(n) = (b(n-1)-1)/2 + 2^e
 *            with the least e >= 0 such that b(n) is not an earlier term and
 *            has at least three 1-bits. a(n) = index of the n-th occurrence
 *            of 7 in b, counting from a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A084449_tab = NULL;
static size_t A084449_cap = 0;
static size_t A084449_cnt = 0;

static seq_t *A084449_pos = NULL;
static size_t A084449_pos_len = 0;
static size_t A084449_pos_cap = 0;

static seq_t A084449_cur = 0;
static seq_t A084449_idx = 0;
static bool A084449_init = false;

static size_t A084449_hash(seq_t x, size_t cap)
{
    uint64_t lo = (uint64_t)(x & (seq_t)0xFFFFFFFFu);
    uint64_t hi = (uint64_t)((x >> 16 >> 16) & (seq_t)0xFFFFFFFFu);
    uint64_t h = (lo ^ (hi * 0xC2B2AE3D27D4EB4FULL)) * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(cap - 1));
}

static void A084449_insert_raw(seq_t *tab, size_t cap, seq_t x)
{
    size_t h = A084449_hash(x, cap);
    while (tab[h] != 0) h = (h + 1) & (cap - 1);
    tab[h] = x;
}

static bool A084449_contains(seq_t x)
{
    size_t h = A084449_hash(x, A084449_cap);
    while (A084449_tab[h] != 0)
    {
        if (A084449_tab[h] == x) return true;
        h = (h + 1) & (A084449_cap - 1);
    }
    return false;
}

/* insert x into the set of seen terms (no-op if already present; x != 0) */
static void A084449_insert(seq_t x)
{
    if (A084449_contains(x)) return;
    if ((A084449_cnt + 1) * 2 > A084449_cap)
    {
        size_t ncap = A084449_cap * 2;
        seq_t *nt = mem_new(seq_t, ncap);
        if (!nt) exit(EXIT_FAILURE);
        for (size_t i = 0; i < A084449_cap; ++i)
            if (A084449_tab[i] != 0) A084449_insert_raw(nt, ncap, A084449_tab[i]);
        mem_free(A084449_tab);
        A084449_tab = nt;
        A084449_cap = ncap;
    }
    A084449_insert_raw(A084449_tab, A084449_cap, x);
    A084449_cnt++;
}

static int A084449_popcount(seq_t x)
{
    int c = 0;
    while (x != 0)
    {
        x &= x - 1;
        c++;
    }
    return c;
}

static void A084449_push(seq_t p)
{
    if (A084449_pos_len == A084449_pos_cap)
    {
        size_t nc = A084449_pos_cap ? A084449_pos_cap * 2 : 256;
        seq_t *t = mem_resize(seq_t, A084449_pos, nc);
        if (!t) exit(EXIT_FAILURE);
        A084449_pos = t;
        A084449_pos_cap = nc;
    }
    A084449_pos[A084449_pos_len++] = p;
}

seq_t a(idx_t n)
{
    if (!A084449_init)
    {
        A084449_init = true;
        A084449_cap = 1024;
        A084449_tab = mem_new(seq_t, A084449_cap);
        if (!A084449_tab) exit(EXIT_FAILURE);
        A084449_cur = 7;
        A084449_idx = 0;
        A084449_insert((seq_t)7);
        A084449_push((seq_t)0);
    }
    while (A084449_pos_len <= (size_t)n)
    {
        seq_t prev = A084449_cur;
        seq_t next;
        if ((prev & 1) == 0)
        {
            next = prev >> 1;
        }
        else
        {
            seq_t base = (prev - 1) >> 1;
            seq_t p2 = 1;
            while (1)
            {
                seq_t cand = base + p2;
                if (!A084449_contains(cand) && A084449_popcount(cand) >= 3)
                {
                    next = cand;
                    break;
                }
                p2 <<= 1;
            }
        }
        A084449_insert(next);
        A084449_cur = next;
        A084449_idx++;
        if (next == 7) A084449_push(A084449_idx);
    }
    return A084449_pos[(size_t)n];
}
