/*
 * A050082  Numbers k such that A050080(k) < A050080(k+1).
 * https://oeis.org/A050082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A050080(k) < A050080(k+1); A050080(1)=1,
 *            A050080(2)=4, A050080(n) = floor(A050080(n-1)/3) if that is
 *            nonzero and not an earlier term, else 4*A050080(n-1). Computed
 *            by direct simulation
 * Range:     u8 n <= 11, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *t_cache = NULL;   /* t_cache[1..t_len] = A050080 terms */
static idx_t t_len = 0, t_cap = 0;
static idx_t *ks = NULL;        /* found k values, ks[1..k_cnt] */
static idx_t k_cnt = 0, k_cap = 0;
static idx_t next_k = 1;        /* next k to test */

static void t_grow(idx_t nc)
{
    seq_t *tmp = mem_new(seq_t, (nc + 1));
    if (!tmp) abort();
    for (idx_t i = 1; i <= t_len; ++i) tmp[i] = t_cache[i];
    if (t_cache) mem_free(t_cache);
    t_cache = tmp;
    t_cap = nc;
}

static void t_next(void)
{
    idx_t i = t_len + 1;
    if (i > t_cap) t_grow(t_cap == 0 ? 64 : t_cap * 2);
    if (i == 1) t_cache[1] = (seq_t)1;
    else if (i == 2) t_cache[2] = (seq_t)4;
    else {
        seq_t prev = t_cache[i - 1];
        seq_t cand = prev / (seq_t)3;
        bool rep = (cand == 0);
        if (!rep) {
            for (idx_t j = 1; j + 2 <= i; ++j) {
                if (t_cache[j] == cand) { rep = true; break; }
            }
        }
        t_cache[i] = rep ? prev * (seq_t)4 : cand;
    }
    t_len = i;
}

seq_t a(idx_t n)
{
    while (k_cnt < n) {
        while (t_len < next_k + 1) t_next();
        if (t_cache[next_k] < t_cache[next_k + 1]) {
            if (k_cnt + 1 >= k_cap) {
                idx_t nc = k_cap == 0 ? 64 : k_cap * 2;
                idx_t *tmp = (idx_t *)malloc((size_t)(nc + 1) * sizeof(idx_t));
                if (!tmp) abort();
                for (idx_t i = 1; i <= k_cnt; ++i) tmp[i] = ks[i];
                if (ks) free(ks);
                ks = tmp;
                k_cap = nc;
            }
            ks[++k_cnt] = next_k;
        }
        ++next_k;
    }
    return (seq_t)ks[n];
}
