/*
 * A050103  Numbers k such that A050100(k) > A050100(k+1).
 * https://oeis.org/A050103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050100(k) > A050100(k+1); A050100:
 *            b(1)=1, b(n)=floor(b(n-1)/3) if positive and not already
 *            present, else 10*b(n-1). Computed by direct simulation
 * Range:     u8 n <= 3, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050103_s = NULL;
static size_t A050103_len = 0, A050103_cap = 0;
static idx_t *A050103_d = NULL;
static size_t A050103_dlen = 0, A050103_dcap = 0;

static void A050103_extend(void)
{
    if (A050103_len == A050103_cap) {
        size_t nc = A050103_cap ? A050103_cap * 2 : 64;
        if (A050103_s == NULL) A050103_s = mem_new(seq_t, nc);
        else A050103_s = mem_resize(seq_t, A050103_s, nc);
        A050103_cap = nc;
    }
    if (A050103_len == 0) {
        A050103_s[0] = (seq_t)1;
        A050103_len = 1;
        return;
    }
    seq_t prev = A050103_s[A050103_len - 1];
    seq_t cand = prev / (seq_t)3;
    bool used = !(cand > (seq_t)0);
    if (!used) {
        for (size_t i = 0; i < A050103_len; ++i) {
            if (A050103_s[i] == cand) { used = true; break; }
        }
    }
    seq_t nv = used ? prev * (seq_t)10 : cand;
    A050103_s[A050103_len] = nv;
    if (prev > nv) {
        if (A050103_dlen == A050103_dcap) {
            size_t nc = A050103_dcap ? A050103_dcap * 2 : 64;
            if (A050103_d == NULL) A050103_d = mem_new(idx_t, nc);
            else A050103_d = mem_resize(idx_t, A050103_d, nc);
            A050103_dcap = nc;
        }
        A050103_d[A050103_dlen++] = (idx_t)A050103_len; /* 1-based index of prev */
    }
    ++A050103_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while ((idx_t)A050103_dlen < n) A050103_extend();
    return (seq_t)A050103_d[n - 1];
}
