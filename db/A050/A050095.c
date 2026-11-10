/*
 * A050095  Numbers k such that A050092(k) > A050092(k+1).
 * https://oeis.org/A050095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050092(k) > A050092(k+1), where
 *            A050092(1) = 1 and A050092(m) = floor(A050092(m-1)/3) if
 *            positive and not yet in the sequence, else 7*A050092(m-1);
 *            computed by direct simulation
 * Range:     u8 n <= 12, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050092_cache = NULL;
static size_t A050092_size = 0;
static size_t A050092_cap = 0;

static idx_t *A050095_desc = NULL;
static size_t A050095_dcnt = 0;
static size_t A050095_dcap = 0;

static bool A050092_seen(seq_t v) {
    for (size_t i = 0; i < A050092_size; ++i)
        if (A050092_cache[i] == v)
            return true;
    return false;
}

static void A050092_push(void) {
    if (A050092_size == A050092_cap) {
        size_t nc = A050092_cap ? A050092_cap * 2 : 64;
        A050092_cache = mem_resize(seq_t, A050092_cache, nc);
        A050092_cap = nc;
    }
    seq_t next;
    if (A050092_size == 0) {
        next = (seq_t)1;
    } else {
        seq_t prev = A050092_cache[A050092_size - 1];
        seq_t d = prev / 3;
        if (d > (seq_t)0 && !A050092_seen(d))
            next = d;
        else
            next = prev * (seq_t)7;
    }
    A050092_cache[A050092_size++] = next;
}

seq_t a(idx_t n) {
    while (A050095_dcnt < (size_t)n) {
        /* compare terms k and k+1, where k = A050092_size (1-based) after generating k+1 terms */
        if (A050092_size == 0)
            A050092_push();
        A050092_push();
        size_t k = A050092_size - 1; /* 1-based index of the earlier term */
        if (A050092_cache[k - 1] > A050092_cache[k]) {
            if (A050095_dcnt == A050095_dcap) {
                size_t nc = A050095_dcap ? A050095_dcap * 2 : 64;
                A050095_desc = (idx_t *)realloc(A050095_desc, nc * sizeof(idx_t));
                A050095_dcap = nc;
            }
            A050095_desc[A050095_dcnt++] = (idx_t)k;
        }
    }
    return (seq_t)A050095_desc[(size_t)n - 1];
}
