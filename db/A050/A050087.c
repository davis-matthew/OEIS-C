/*
 * A050087  Numbers k such that A050084(k) > A050084(k+1).
 * https://oeis.org/A050087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050084(k) > A050084(k+1); computed by
 *            generating A050084 (A050084(1)=1, then floor(prev/3) if new and
 *            nonzero, else 5*prev) and recording each index where a division
 *            step occurs (a descent)
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050087_t = NULL;     /* terms of A050084, 0-based */
static idx_t A050087_tlen = 0, A050087_tcap = 0;
static seq_t *A050087_d = NULL;     /* descent indices */
static idx_t A050087_dlen = 0, A050087_dcap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A050087_tlen == 0) {
        A050087_tcap = 64;
        A050087_t = mem_new(seq_t, A050087_tcap);
        A050087_t[0] = (seq_t)1;
        A050087_tlen = 1;
        A050087_dcap = 64;
        A050087_d = mem_new(seq_t, A050087_dcap);
    }
    while (A050087_dlen < n) {
        if (A050087_tlen >= A050087_tcap) {
            idx_t nc = A050087_tcap * 2;
            seq_t *tmp = mem_resize(seq_t, A050087_t, nc);
            if (!tmp) abort();
            A050087_t = tmp;
            A050087_tcap = nc;
        }
        idx_t len = A050087_tlen;
        seq_t prev = A050087_t[len - 1];
        seq_t cand = prev / 3;
        bool dup = (cand == (seq_t)0);
        for (idx_t i = 0; !dup && i + 1 < len; ++i)
            if (A050087_t[i] == cand) dup = true;
        if (dup) {
            A050087_t[len] = prev * (seq_t)5;
        } else {
            A050087_t[len] = cand;
            if (A050087_dlen >= A050087_dcap) {
                idx_t nc = A050087_dcap * 2;
                seq_t *tmp = mem_resize(seq_t, A050087_d, nc);
                if (!tmp) abort();
                A050087_d = tmp;
                A050087_dcap = nc;
            }
            A050087_d[A050087_dlen++] = (seq_t)len;  /* 1-based index of prev */
        }
        A050087_tlen = len + 1;
    }
    return A050087_d[n - 1];
}
