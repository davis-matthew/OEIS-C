/*
 * A006540  Denominators of worst case for Engel expansion.
 * https://oeis.org/A006540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = smallest q such that some p/q (0 < p <= q) has an
 *            Engel expansion of at least n+1 terms. The Engel step p/q ->
 *            (a*p - q)/q, with a = ceil(q/p), keeps the denominator q, so the
 *            numerator map is p -> (-q) mod p and the expansion ends when it
 *            reaches 0. So len_q(p) = 1 + len_q((-q) mo...
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *eng_len = NULL;
static uint64_t eng_cap = 0;
static uint64_t *eng_ans = NULL;   /* eng_ans[L] = first q reaching length >= L */
static uint64_t eng_anscap = 0;
static uint64_t eng_maxL = 0;
static uint64_t eng_q = 0;

static void eng_step(void) {
    uint64_t q = ++eng_q, p, m = 0;
    if (q + 1 > eng_cap) {
        uint64_t nc = eng_cap ? eng_cap : 64;
        while (nc < q + 1) nc *= 2;
        eng_len = (uint32_t *)realloc(eng_len, nc * sizeof(uint32_t));
        eng_cap = nc;
    }
    eng_len[0] = 0;
    for (p = 1; p <= q; p++) {
        uint64_t r = q % p;
        uint64_t nx = r ? p - r : 0;
        uint32_t L = 1 + eng_len[nx];
        eng_len[p] = L;
        if (L > m) m = L;
    }
    while (eng_maxL < m) {
        eng_maxL++;
        if (eng_maxL + 1 > eng_anscap) {
            uint64_t nc = eng_anscap ? eng_anscap * 2 : 64;
            while (nc < eng_maxL + 1) nc *= 2;
            eng_ans = (uint64_t *)realloc(eng_ans, nc * sizeof(uint64_t));
            eng_anscap = nc;
        }
        eng_ans[eng_maxL] = q;
    }
}

seq_t a(idx_t n) {
    uint64_t target = (uint64_t)n + 1;
    while (eng_maxL < target) eng_step();
    return (seq_t)eng_ans[target];
}
