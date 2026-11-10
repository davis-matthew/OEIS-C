/*
 * A050097  a(n)=position of n in A050096.
 * https://oeis.org/A050097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050096, where A050096(1) = 1 and
 *            A050096(k) = floor(A050096(k-1)/3) if positive and not yet seen,
 *            else 8*A050096(k-1). Computed by generating A050096 until n
 *            appears
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A050097_c = NULL;
static size_t A050097_len = 0, A050097_cap = 0;

static void A050097_push(seq_t v)
{
    if (A050097_len == A050097_cap) {
        size_t nc = A050097_cap ? A050097_cap * 2 : 256;
        seq_t *p = mem_new(seq_t, nc);
        for (size_t i = 0; i < A050097_len; ++i) p[i] = A050097_c[i];
        if (A050097_c) mem_free(A050097_c);
        A050097_c = p;
        A050097_cap = nc;
    }
    A050097_c[A050097_len++] = v;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    if (A050097_len == 0) A050097_push((seq_t)1);
    for (size_t i = 0; i < A050097_len; ++i)
        if (A050097_c[i] == target) return (seq_t)(i + 1);
    while (A050097_len < 40000) {
        seq_t prev = A050097_c[A050097_len - 1];
        seq_t cand = prev / 3;
        bool ok = cand > 0;
        if (ok) {
            for (size_t i = 0; i < A050097_len; ++i)
                if (A050097_c[i] == cand) { ok = false; break; }
        }
        seq_t next = ok ? cand : prev * 8;
        A050097_push(next);
        if (next == target) return (seq_t)A050097_len;
    }
    return (seq_t)0;
}
