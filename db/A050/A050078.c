/*
 * A050078  Numbers k such that A050076(k) < A050076(k+1).
 * https://oeis.org/A050078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050076(k) < A050076(k+1), where
 *            A050076: b(1)=1, b(n)=floor(b(n-1)/3) if that is nonzero and
 *            unused, else 2*b(n-1); computed by direct simulation
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050076_terms = NULL;
static size_t A050076_len = 0, A050076_cap = 0;
static size_t *A050078_asc = NULL;
static size_t A050078_cnt = 0, A050078_cap = 0;

static bool A050076_used(seq_t v)
{
    for (size_t i = 0; i < A050076_len; ++i)
        if (A050076_terms[i] == v)
            return true;
    return false;
}

static void A050076_push(seq_t v)
{
    if (A050076_len >= A050076_cap) {
        size_t nc = A050076_cap ? A050076_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A050076_terms, nc);
        if (!t) exit(EXIT_FAILURE);
        A050076_terms = t;
        A050076_cap = nc;
    }
    A050076_terms[A050076_len++] = v;
}

static void A050078_push(size_t k)
{
    if (A050078_cnt >= A050078_cap) {
        size_t nc = A050078_cap ? A050078_cap * 2 : 64;
        size_t *t = (size_t *)realloc(A050078_asc, nc * sizeof(size_t));
        if (!t) exit(EXIT_FAILURE);
        A050078_asc = t;
        A050078_cap = nc;
    }
    A050078_asc[A050078_cnt++] = k;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    size_t want = (size_t)n;
    if (A050076_len == 0)
        A050076_push((seq_t)1);
    while (A050078_cnt < want) {
        seq_t prev = A050076_terms[A050076_len - 1];
        seq_t cand = prev / 3;
        if (cand == 0 || A050076_used(cand)) {
            A050078_push(A050076_len);      /* 1-based index of prev */
            A050076_push(prev * 2);
        } else {
            A050076_push(cand);
        }
    }
    return (seq_t)A050078_asc[want - 1];
}
