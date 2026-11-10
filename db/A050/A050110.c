/*
 * A050110  Numbers k such that A050108(k) < A050108(k+1).
 * https://oeis.org/A050110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050108(k) < A050108(k+1), i.e. k where
 *            A050108(k+1) = 5*A050108(k); A050108(1)=1,
 *            A050108(m)=floor(A050108(m-1)/4) if positive and unseen, else
 *            5*A050108(m-1). Computed by simulation
 * Range:     u8 n <= 11, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050110_terms = NULL;
static size_t A050110_tcap = 0;
static size_t A050110_tcnt = 0;
static idx_t *A050110_res = NULL;
static size_t A050110_rcap = 0;
static size_t A050110_rcnt = 0;

seq_t a(idx_t n)
{
    if (A050110_tcnt == 0) {
        A050110_tcap = 256;
        A050110_terms = mem_resize(seq_t, A050110_terms, A050110_tcap);
        A050110_rcap = 64;
        A050110_res = mem_resize(idx_t, A050110_res, A050110_rcap);
        if (!A050110_terms || !A050110_res) abort();
        A050110_terms[0] = 1;
        A050110_tcnt = 1;
    }
    while (A050110_rcnt < (size_t)n) {
        size_t cnt = A050110_tcnt;
        if (cnt >= A050110_tcap) {
            size_t ns = A050110_tcap * 2;
            seq_t *t = mem_resize(seq_t, A050110_terms, ns);
            if (!t) abort();
            A050110_terms = t;
            A050110_tcap = ns;
        }
        seq_t prev = A050110_terms[cnt - 1];
        seq_t cand = prev / 4;
        bool seen = false;
        if (cand > 0) {
            for (size_t j = 0; j < cnt; ++j) {
                if (A050110_terms[j] == cand) { seen = true; break; }
            }
        }
        if (cand > 0 && !seen) {
            A050110_terms[cnt] = cand;
        } else {
            A050110_terms[cnt] = 5 * prev;
            if (A050110_rcnt >= A050110_rcap) {
                size_t nr = A050110_rcap * 2;
                idx_t *r = mem_resize(idx_t, A050110_res, nr);
                if (!r) abort();
                A050110_res = r;
                A050110_rcap = nr;
            }
            A050110_res[A050110_rcnt++] = (idx_t)cnt;
        }
        A050110_tcnt = cnt + 1;
    }
    return (seq_t)A050110_res[(size_t)n - 1];
}
