/*
 * A050019  Numbers k such that A050016(k) > A050016(k+1).
 * https://oeis.org/A050019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A050016(k) > A050016(k+1); A050016(1)=1,
 *            A050016(m)=floor(A050016(m-1)/2) if positive and not yet in
 *            sequence, else 9*A050016(m-1). Computed by direct generation and
 *            scan
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050019_t = NULL;     /* terms of A050016, 1-indexed */
static size_t A050019_tcap = 0, A050019_tlen = 0;
static idx_t *A050019_d = NULL;     /* descent positions, 1-indexed */
static size_t A050019_dcap = 0, A050019_dlen = 0;
static size_t A050019_k = 1;        /* next k to test */

static int A050019_extend(void)
{
    seq_t last = A050019_t[A050019_tlen];
    seq_t cand = last / (seq_t)2;
    int seen = (cand == (seq_t)0);
    if (!seen) {
        for (size_t i = 1; i <= A050019_tlen; ++i) {
            if (A050019_t[i] == cand) { seen = 1; break; }
        }
    }
    seq_t next = seen ? (seq_t)(last * (seq_t)9) : cand;
    if (A050019_tlen + 2 >= A050019_tcap) {
        size_t nc = A050019_tcap * 2;
        seq_t *tmp = mem_resize(seq_t, A050019_t, nc);
        if (!tmp) return 0;
        A050019_t = tmp;
        A050019_tcap = nc;
    }
    A050019_t[++A050019_tlen] = next;
    return 1;
}

seq_t a(idx_t n)
{
    if (A050019_t == NULL) {
        A050019_tcap = 16;
        A050019_t = mem_new(seq_t, A050019_tcap);
        if (!A050019_t) return (seq_t)0;
        A050019_t[1] = (seq_t)1;
        A050019_tlen = 1;
        A050019_dcap = 16;
        A050019_d = mem_new(idx_t, A050019_dcap);
        if (!A050019_d) return (seq_t)0;
        A050019_dlen = 0;
        A050019_k = 1;
    }
    while (A050019_dlen < (size_t)n) {
        while (A050019_tlen < A050019_k + 1) {
            if (!A050019_extend()) return (seq_t)0;
        }
        if (A050019_t[A050019_k] > A050019_t[A050019_k + 1]) {
            if (A050019_dlen + 2 >= A050019_dcap) {
                size_t nc = A050019_dcap * 2;
                idx_t *tmp = mem_resize(idx_t, A050019_d, nc);
                if (!tmp) return (seq_t)0;
                A050019_d = tmp;
                A050019_dcap = nc;
            }
            A050019_d[++A050019_dlen] = (idx_t)A050019_k;
        }
        ++A050019_k;
    }
    return (seq_t)A050019_d[n];
}
