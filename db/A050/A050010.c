/*
 * A050010  Numbers k such that A050008(k) < A050008(k+1).
 * https://oeis.org/A050010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A050008(k) < A050008(k+1); computed by
 *            generating A050008 (a(1)=1, a(n)=floor(a(n-1)/2) if positive and
 *            unused, else 6*a(n-1)) and recording ascents
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A050008_v = NULL;   /* 1-based values */
static size_t A050008_len = 0;
static size_t A050008_cap = 0;
static idx_t *A050010_asc = NULL; /* 1-based list of ascent indices */
static size_t A050010_cnt = 0;
static size_t A050010_cap = 0;

static void A050010_step(void)
{
    if (A050008_len == 0) {
        A050008_cap = 64;
        A050008_v = mem_new(seq_t, A050008_cap);
        A050008_v[1] = (seq_t)1;
        A050008_len = 1;
        return;
    }
    if (A050008_len + 2 > A050008_cap) {
        A050008_cap *= 2;
        A050008_v = mem_resize(seq_t, A050008_v, A050008_cap);
    }
    size_t i = A050008_len + 1;
    seq_t prev = A050008_v[i - 1];
    seq_t cand = prev / (seq_t)2;
    bool use = false;
    if (cand > (seq_t)0) {
        bool found = false;
        for (size_t j = 1; j < i; ++j) {
            if (A050008_v[j] == cand) { found = true; break; }
        }
        use = !found;
    }
    A050008_v[i] = use ? cand : prev * (seq_t)6;
    A050008_len = i;

    if (A050008_v[i - 1] < A050008_v[i]) {
        if (A050010_cnt + 2 > A050010_cap) {
            size_t nc = A050010_cap ? A050010_cap * 2 : 64;
            idx_t *p = (idx_t *)malloc(nc * sizeof(idx_t));
            for (size_t k = 1; k <= A050010_cnt; ++k) p[k] = A050010_asc[k];
            free(A050010_asc);
            A050010_asc = p;
            A050010_cap = nc;
        }
        A050010_asc[++A050010_cnt] = (idx_t)(i - 1);
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    while ((idx_t)A050010_cnt < n)
        A050010_step();
    return (seq_t)A050010_asc[(size_t)n];
}
