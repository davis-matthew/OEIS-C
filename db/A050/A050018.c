/*
 * A050018  Numbers k such that A050016(k) < A050016(k+1).
 * https://oeis.org/A050018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A050016(k) < A050016(k+1), where
 *            A050016(1) = 1, A050016(m) = floor(A050016(m-1)/2) if that is
 *            positive and not already in the sequence, else 9*A050016(m-1).
 *            Computed by direct simulation with a cache
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050018_vals = NULL;   /* A050016 values, 1-indexed */
static size_t A050018_vcap = 0, A050018_vlen = 0;
static idx_t *A050018_asc = NULL;    /* ascent positions */
static size_t A050018_acap = 0, A050018_alen = 0;

static void A050018_step(void)
{
    if (A050018_vals == NULL) {
        A050018_vcap = 64;
        A050018_vals = mem_new(seq_t, A050018_vcap);
        A050018_vlen = 1;
        A050018_vals[1] = (seq_t)1;
        A050018_acap = 64;
        A050018_asc = (idx_t *)malloc(A050018_acap * sizeof(idx_t));
        A050018_alen = 0;
    }
    size_t len = A050018_vlen;
    seq_t last = A050018_vals[len];
    seq_t cand = last / (seq_t)2;
    bool seen = (cand == (seq_t)0);
    if (!seen) {
        for (size_t i = 1; i <= len; ++i) {
            if (A050018_vals[i] == cand) { seen = true; break; }
        }
    }
    seq_t next = seen ? last * (seq_t)9 : cand;
    if (len + 2 >= A050018_vcap) {
        size_t nc = A050018_vcap * 2;
        A050018_vals = mem_resize(seq_t, A050018_vals, nc);
        A050018_vcap = nc;
    }
    A050018_vals[++A050018_vlen] = next;
    if (last < next) {
        if (A050018_alen >= A050018_acap) {
            A050018_acap *= 2;
            A050018_asc = (idx_t *)realloc(A050018_asc, A050018_acap * sizeof(idx_t));
        }
        A050018_asc[A050018_alen++] = (idx_t)len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A050018_vals == NULL) A050018_step();
    while (A050018_alen < (size_t)n) A050018_step();
    return (seq_t)A050018_asc[n - 1];
}
