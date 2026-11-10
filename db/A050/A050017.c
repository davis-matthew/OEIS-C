/*
 * A050017  a(n) = position of n in A050016.
 * https://oeis.org/A050017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of n in A050016, where A050016: b(1) = 1; b(k) =
 *            floor(b(k-1)/2) if that is positive and not yet in the sequence,
 *            else 9*b(k-1). Computed by generating A050016 until n appears
 * Range:     u8 n <= 16, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050017_terms = NULL;
static size_t A050017_cap = 0;
static size_t A050017_len = 0;
static unsigned char A050017_small[65536];
static seq_t *A050017_large = NULL;
static size_t A050017_lcap = 0;
static size_t A050017_llen = 0;

static bool A050017_add(seq_t v)
{
    if (A050017_len + 1 >= A050017_cap) {
        size_t nc = A050017_cap ? A050017_cap * 2 : 1024;
        seq_t *t = mem_resize(seq_t, A050017_terms, nc);
        if (!t) return false;
        A050017_terms = t;
        A050017_cap = nc;
    }
    A050017_terms[++A050017_len] = v;
    if (v < (seq_t)65536) {
        A050017_small[(size_t)(long long)v] = 1;
    } else {
        if (A050017_llen + 1 >= A050017_lcap) {
            size_t nc = A050017_lcap ? A050017_lcap * 2 : 256;
            seq_t *t = mem_resize(seq_t, A050017_large, nc);
            if (!t) return false;
            A050017_large = t;
            A050017_lcap = nc;
        }
        A050017_large[A050017_llen++] = v;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (A050017_terms == NULL) {
        A050017_cap = 1024;
        A050017_terms = mem_new(seq_t, A050017_cap);
        if (!A050017_terms) return (seq_t)0;
        if (!A050017_add((seq_t)1)) return (seq_t)0;
    }
    seq_t target = (seq_t)n;
    for (size_t i = 1; i <= A050017_len; ++i) {
        if (A050017_terms[i] == target) return (seq_t)i;
    }
    for (;;) {
        seq_t last = A050017_terms[A050017_len];
        seq_t cand = last / (seq_t)2;
        bool seen = false;
        if (cand == (seq_t)0) {
            seen = true;
        } else if (cand < (seq_t)65536) {
            seen = A050017_small[(size_t)(long long)cand] != 0;
        } else {
            for (size_t i = 0; i < A050017_llen; ++i) {
                if (A050017_large[i] == cand) { seen = true; break; }
            }
        }
        seq_t next = seen ? last * (seq_t)9 : cand;
        if (!A050017_add(next)) return (seq_t)0;
        if (next == target) return (seq_t)A050017_len;
    }
}
