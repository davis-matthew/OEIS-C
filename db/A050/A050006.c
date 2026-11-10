/*
 * A050006  Numbers k such that A050004(k) < A050004(k+1).
 * https://oeis.org/A050006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A050004(k) < A050004(k+1), where
 *            A050004(1) = 1, A050004(n) = floor(A050004(n-1)/2) if this is
 *            not 0 or among A050004(1..n-2), else 5*A050004(n-1); computed by
 *            direct scan
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050006_B = NULL;
static size_t A050006_blen = 0, A050006_bcap = 0;
static idx_t *A050006_R = NULL;
static size_t A050006_rlen = 0, A050006_rcap = 0;
static idx_t A050006_next = 1;

static void A050006_ensure_b(size_t len) {
    if (len > A050006_bcap) {
        size_t cap = A050006_bcap > 0 ? A050006_bcap : 64;
        while (cap < len) cap <<= 1;
        seq_t *p = mem_resize(seq_t, A050006_B, cap);
        if (!p) abort();
        A050006_B = p;
        A050006_bcap = cap;
    }
    while (A050006_blen < len) {
        if (A050006_blen == 0) {
            A050006_B[0] = (seq_t)1;
            A050006_blen = 1;
            continue;
        }
        seq_t prev = A050006_B[A050006_blen - 1];
        seq_t cand = prev / 2;
        bool mul5 = false;
        if (cand == 0) {
            mul5 = true;
        } else {
            for (size_t j = 0; j + 1 < A050006_blen; ++j) {
                if (A050006_B[j] == cand) { mul5 = true; break; }
            }
        }
        A050006_B[A050006_blen] = mul5 ? (seq_t)(prev * 5) : cand;
        ++A050006_blen;
    }
}

seq_t a(idx_t n) {
    while (A050006_rlen < (size_t)n) {
        A050006_ensure_b((size_t)A050006_next + 1);
        size_t k = (size_t)A050006_next;
        if (A050006_B[k - 1] < A050006_B[k]) {
            if (A050006_rlen >= A050006_rcap) {
                size_t cap = A050006_rcap > 0 ? A050006_rcap * 2 : 64;
                idx_t *p = (idx_t *)realloc(A050006_R, cap * sizeof(idx_t));
                if (!p) abort();
                A050006_R = p;
                A050006_rcap = cap;
            }
            A050006_R[A050006_rlen++] = A050006_next;
        }
        ++A050006_next;
    }
    return (seq_t)A050006_R[(size_t)n - 1];
}
