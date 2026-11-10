/*
 * A092842  a(n) is the solution of the equation A073666(x) = n (A073666(a(n))
 *          = n).
 * https://oeis.org/A092842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A073666(k) = n; computed by running the
 *            greedy construction of A073666 (a(1)=1, then smallest unused m
 *            with prev*m+1 prime) until n appears
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A092842_pos = NULL;   /* pos[v] = 1-based position of v, 0 if unseen */
static size_t A092842_cap = 0;
static uint64_t A092842_len = 0;       /* terms generated so far */
static uint64_t A092842_prev = 0;

static void A092842_grow(size_t need)
{
    if (need < A092842_cap) return;
    size_t nc = A092842_cap ? A092842_cap : 64;
    while (nc <= need) nc <<= 1;
    A092842_pos = (uint64_t *)realloc(A092842_pos, nc * sizeof(uint64_t));
    for (size_t j = A092842_cap; j < nc; ++j) A092842_pos[j] = 0;
    A092842_cap = nc;
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (n == 0) return (seq_t)0;
    A092842_grow(target + 1);
    if (A092842_len == 0) {
        A092842_pos[1] = 1;
        A092842_len = 1;
        A092842_prev = 1;
    }
    while (A092842_pos[target] == 0) {
        uint64_t cand = 1;
        for (;; ++cand) {
            A092842_grow((size_t)cand + 1);
            if (A092842_pos[cand]) continue;
            if (is_prime_u64(A092842_prev * cand + 1)) break;
        }
        A092842_len++;
        A092842_pos[cand] = A092842_len;
        A092842_prev = cand;
    }
    return (seq_t)A092842_pos[target];
}
