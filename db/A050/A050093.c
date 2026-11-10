/*
 * A050093  a(n)=position of n in A050092.
 * https://oeis.org/A050093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050092, where A050092(1) = 1 and for m
 *            > 1, A050092(m) = floor(A050092(m-1)/3) if this is positive and
 *            not yet in the sequence, otherwise 7*A050092(m-1). Computed by
 *            generating A050092 until n appears
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
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

static bool A050092_seen(seq_t v) {
    for (size_t i = 0; i < A050092_size; ++i)
        if (A050092_cache[i] == v)
            return true;
    return false;
}

static void A050092_extend(void) {
    if (A050092_size >= A050092_cap) {
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
    size_t scanned = 0;
    for (;;) {
        for (; scanned < A050092_size; ++scanned)
            if (A050092_cache[scanned] == (seq_t)n)
                return (seq_t)(scanned + 1);
        if (A050092_size >= 30000)
            return (seq_t)0;
        A050092_extend();
    }
}
