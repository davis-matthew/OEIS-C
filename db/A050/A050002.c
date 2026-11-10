/*
 * A050002  Numbers k such that A050000(k) < A050000(k+1).
 * https://oeis.org/A050002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A050000(k) < A050000(k+1); computed by
 *            direct generation of A050000 (A050000(1)=1, halve if new and
 *            nonzero, else triple) and recording the indices of ascents
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t tlen = 0, tcap = 0;
    static size_t *res = NULL;
    static size_t rlen = 0, rcap = 0;

    if (tlen == 0) {
        tcap = 64;
        terms = mem_new(seq_t, tcap);
        terms[0] = (seq_t)1;
        tlen = 1;
        rcap = 64;
        res = (size_t *)malloc(rcap * sizeof(size_t));
    }

    while ((idx_t)rlen < n) {
        seq_t prev = terms[tlen - 1];
        seq_t cand = prev / 2;
        bool ok = (cand != (seq_t)0);
        if (ok) {
            for (size_t i = 0; i < tlen; i++) {
                if (terms[i] == cand) { ok = false; break; }
            }
        }
        seq_t nv = ok ? cand : prev * (seq_t)3;
        if (tlen >= tcap) {
            tcap *= 2;
            terms = mem_resize(seq_t, terms, tcap);
        }
        terms[tlen++] = nv;
        if (prev < nv) {
            if (rlen >= rcap) {
                rcap *= 2;
                res = (size_t *)realloc(res, rcap * sizeof(size_t));
            }
            res[rlen++] = tlen - 1; /* 1-indexed k = index of prev */
        }
    }
    return (seq_t)res[n - 1];
}
