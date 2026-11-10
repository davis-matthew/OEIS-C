/*
 * A050089  a(n) = position of n in A050088.
 * https://oeis.org/A050089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050088, where A050088(1) = 1 and, for k
 *            > 1, A050088(k) = floor(A050088(k-1)/3) if that is positive and
 *            not already a term, else 6*A050088(k-1). Computed by generating
 *            A050088 until n appears
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050089_terms = NULL;
static size_t A050089_len = 0;
static size_t A050089_cap = 0;

static void A050089_push(seq_t v)
{
    if (A050089_cap == 0) {
        A050089_cap = 1024;
        A050089_terms = mem_new(seq_t, A050089_cap);
    } else if (A050089_len == A050089_cap) {
        A050089_cap *= 2;
        A050089_terms = mem_resize(seq_t, A050089_terms, A050089_cap);
    }
    A050089_terms[A050089_len++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t target = (seq_t)n;

    if (A050089_len == 0) A050089_push((seq_t)1);

    /* search already generated terms */
    for (size_t i = 0; i < A050089_len; ++i) {
        if (A050089_terms[i] == target) return (seq_t)(i + 1);
    }

    while (A050089_len < 20000000u) {
        seq_t prev = A050089_terms[A050089_len - 1];
        seq_t cand = prev / 3;
        bool seen = false;
        if (cand > 0) {
            for (size_t i = 0; i < A050089_len; ++i) {
                if (A050089_terms[i] == cand) { seen = true; break; }
            }
        }
        seq_t next;
        if (cand > 0 && !seen) next = cand;
        else next = prev * 6;
        A050089_push(next);
        if (next == target) return (seq_t)A050089_len;
    }
    return 0;
}
