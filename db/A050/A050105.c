/*
 * A050105  a(n)=position of n in A050104.
 * https://oeis.org/A050105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050104, where A050104 has b(1) = 1 and
 *            b(k) = floor(b(k-1)/4) if that is nonzero and not among
 *            b(1..k-2), else 3*b(k-1). Computed by generating A050104 with a
 *            hash set until n appears
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050105_keys = 0;
static idx_t *A050105_pos = 0;
static size_t A050105_cap = 0;
static size_t A050105_cnt = 0;
static seq_t A050105_cur;

static size_t A050105_hash(seq_t x, size_t mask)
{
    unsigned long long h = 1469598103934665603ULL;
    int i;
    for (i = 0; i < 8; i++) {
        unsigned long long chunk = (unsigned long long)(x & (seq_t)0xFF);
        h ^= chunk;
        h *= 1099511628211ULL;
        x = x >> 8;
    }
    h ^= h >> 29;
    return (size_t)(h & (unsigned long long)mask);
}

static void A050105_insert(seq_t key, idx_t pos)
{
    size_t mask = A050105_cap - 1;
    size_t h = A050105_hash(key, mask);
    while (A050105_pos[h] != 0) h = (h + 1) & mask;
    A050105_keys[h] = key;
    A050105_pos[h] = pos;
}

static idx_t A050105_find(seq_t key)
{
    size_t mask, h;
    if (A050105_cap == 0) return 0;
    mask = A050105_cap - 1;
    h = A050105_hash(key, mask);
    while (A050105_pos[h] != 0) {
        if (A050105_keys[h] == key) return A050105_pos[h];
        h = (h + 1) & mask;
    }
    return 0;
}

static void A050105_grow(void)
{
    size_t oldcap = A050105_cap;
    seq_t *ok = A050105_keys;
    idx_t *op = A050105_pos;
    size_t i;
    A050105_cap = oldcap ? oldcap * 2 : 1024;
    A050105_keys = mem_new(seq_t, A050105_cap);
    A050105_pos = mem_new(idx_t, A050105_cap);
    for (i = 0; i < oldcap; i++)
        if (op[i] != 0) A050105_insert(ok[i], op[i]);
    if (oldcap) {
        mem_free(ok);
        mem_free(op);
    }
}

static void A050105_step(void)
{
    if (A050105_cnt == 0) {
        A050105_grow();
        A050105_cur = (seq_t)1;
        A050105_insert(A050105_cur, (idx_t)1);
        A050105_cnt = 1;
        return;
    }
    {
        seq_t c = A050105_cur / (seq_t)4;
        if (c == (seq_t)0 || A050105_find(c) != 0)
            A050105_cur = A050105_cur * (seq_t)3;
        else
            A050105_cur = c;
    }
    if ((A050105_cnt + 1) * 2 > A050105_cap) A050105_grow();
    A050105_cnt++;
    /* if value already present (overflow wrap), skip inserting to keep first position */
    if (A050105_find(A050105_cur) == 0)
        A050105_insert(A050105_cur, (idx_t)A050105_cnt);
}

seq_t a(idx_t n)
{
    idx_t p;
    if (n == 0) return (seq_t)0;
    if (A050105_cnt == 0) A050105_step();
    for (;;) {
        p = A050105_find((seq_t)n);
        if (p != 0) return (seq_t)p;
        if (A050105_cnt >= 20000000u) return (seq_t)0;
        A050105_step();
    }
}
