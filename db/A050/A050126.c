/*
 * A050126  Numbers k such that A050124(k) < A050124(k+1).
 * https://oeis.org/A050126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A050124(1)=1; A050124(n)=floor(A050124(n-1)/5) if that is not
 *            among 0,A050124(1..n-1), else 2*A050124(n-1). a(n) = n-th k such
 *            that A050124(k) < A050124(k+1); computed by direct scan
 * Range:     u8 n <= 22, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050124_t = NULL;
static size_t A050124_len = 0, A050124_cap = 0;
static idx_t *asc_k = NULL;
static size_t asc_len = 0, asc_cap = 0;

static void A050124_extend(void)
{
    if (A050124_len == 0) {
        A050124_cap = 64;
        A050124_t = mem_new(seq_t, A050124_cap);
        A050124_t[0] = (seq_t)1;
        A050124_len = 1;
    }
    if (A050124_len + 1 > A050124_cap) {
        A050124_cap <<= 1;
        A050124_t = mem_resize(seq_t, A050124_t, A050124_cap);
    }
    seq_t prev = A050124_t[A050124_len - 1];
    seq_t cand = prev / (seq_t)5;
    bool used = (cand == (seq_t)0);
    for (size_t i = 0; !used && i < A050124_len; ++i)
        if (A050124_t[i] == cand) used = true;
    seq_t nxt = used ? (seq_t)(prev * (seq_t)2) : cand;
    A050124_t[A050124_len] = nxt;
    if (prev < nxt) {
        if (asc_len == asc_cap) {
            asc_cap = asc_cap ? asc_cap * 2 : 64;
            asc_k = (idx_t *)realloc(asc_k, asc_cap * sizeof(idx_t));
        }
        asc_k[asc_len++] = (idx_t)A050124_len; /* 1-based index k of prev */
    }
    A050124_len++;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while ((idx_t)asc_len < n)
        A050124_extend();
    return (seq_t)asc_k[n - 1];
}
