/*
 * A050085  a(n)=position of n in A050084.
 * https://oeis.org/A050085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050084, where A050084 has a(1) = 1 and,
 *            for n > 1, a(n) = floor(a(n-1)/3) if that is nonzero and not
 *            among a(1..n-2), else 5*a(n-1). Computed by generating A050084
 *            until n appears
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050085_cache = NULL;
static idx_t A050085_cap = 0;
static idx_t A050085_len = 0;

static void A050085_grow(idx_t need)
{
    if (A050085_cap >= need) return;
    idx_t nc = A050085_cap ? A050085_cap << 1 : 64;
    while (nc < need) nc <<= 1;
    seq_t *t = mem_resize(seq_t, A050085_cache, nc);
    if (!t) abort();
    A050085_cache = t;
    A050085_cap = nc;
}

static bool A050085_contains(idx_t len, seq_t v)
{
    for (idx_t i = 0; i < len; ++i)
        if (A050085_cache[i] == v) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A050085_len == 0) {
        A050085_grow(1);
        A050085_cache[0] = (seq_t)1;
        A050085_len = 1;
    }
    seq_t target = (seq_t)n;
    for (idx_t i = 0; i < A050085_len; ++i)
        if (A050085_cache[i] == target) return (seq_t)(i + 1);

    const idx_t LIMIT = 400000;
    while (A050085_len < LIMIT) {
        idx_t idx = A050085_len;
        A050085_grow(idx + 1);
        seq_t prev = A050085_cache[idx - 1];
        seq_t cand = prev / 3;
        bool dup = (cand == (seq_t)0) || A050085_contains(idx - 1, cand);
        seq_t val = dup ? prev * (seq_t)5 : cand;
        A050085_cache[idx] = val;
        A050085_len = idx + 1;
        if (val == target) return (seq_t)(idx + 1);
    }
    return (seq_t)0;
}
