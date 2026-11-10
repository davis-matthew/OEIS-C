/*
 * A054087  s(3n-2), s=A054086; also a bisection of A003511.
 * https://oeis.org/A054087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(3n-2) where s = A054086: s(1)=1; for k>=1, p(k)=least
 *            unused, s(3k-1)=p(k)+k, s(3k)=p(k), s(3k+1)=least unused
 *            (computed by simulation)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void A054086_grow_used(size_t need, unsigned char **used, size_t *used_cap)
{
    if (*used_cap >= need)
        return;
    size_t new_cap = *used_cap ? *used_cap : 16;
    while (new_cap < need)
        new_cap *= 2;
    unsigned char *new_used = (unsigned char *)realloc(*used, new_cap);
    if (!new_used)
        exit(1);
    memset(new_used + *used_cap, 0, new_cap - *used_cap);
    *used = new_used;
    *used_cap = new_cap;
}

static void A054086_grow_cache(size_t need, seq_t **cache, size_t *cache_cap)
{
    if (*cache_cap >= need)
        return;
    size_t new_cap = *cache_cap ? *cache_cap : 32;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = mem_resize(seq_t, *cache, new_cap);
    if (!new_cache)
        exit(1);
    *cache = new_cache;
    *cache_cap = new_cap;
}

static seq_t A054086_a(idx_t n)
{
    if (n == 0)
        return 0;

    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static unsigned char *used = NULL;
    static size_t used_cap = 0;

    static idx_t min_unused = 1;

    if (!seq) {
        seq_cap = 32;
        seq = mem_new(seq_t, seq_cap);
        if (!seq) exit(1);
        seq[0] = 1;
        seq_len = 1;

        used_cap = 64;
        used = (unsigned char *)calloc(used_cap, 1);
        if (!used) exit(1);
        used[1] = 1;
    }

    while (seq_len < (size_t)n) {
        idx_t k = ((idx_t)seq_len - 1) / 3 + 1;

        A054086_grow_used((size_t)min_unused + 1, &used, &used_cap);
        while (used[(size_t)min_unused])
            min_unused++;
        idx_t p = min_unused;
        used[(size_t)min_unused] = 1;

        idx_t q_idx = p + k;
        A054086_grow_used((size_t)q_idx + 1, &used, &used_cap);

        A054086_grow_cache(seq_len + 3, &seq, &seq_cap);

        seq[seq_len++] = (seq_t)q_idx;
        seq[seq_len++] = (seq_t)p;
        used[(size_t)q_idx] = 1;

        A054086_grow_used((size_t)min_unused + 1, &used, &used_cap);
        while (used[(size_t)min_unused])
            min_unused++;
        seq[seq_len++] = (seq_t)min_unused;
        used[(size_t)min_unused] = 1;
    }

    return seq[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A054086_a(3 * n - 2);
}
