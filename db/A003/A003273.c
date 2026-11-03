/*
 * A003273  Congruent numbers: positive integers k for which there exists a
 *          right triangle having area k and rational sides.
 * https://oeis.org/A003273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search over k = 1, 2, 3, ...: k is a term iff its squarefree
 *            part s is congruent, decided by Tunnell's criterion: for odd s,
 *            2*#{s = 2x^2+y^2+32z^2} = #{s = 2x^2+y^2+8z^2}; for even s,
 *            2*#{s = 8x^2+2y^2+64z^2} = #{s = 8x^2+2y^2+16z^2}
 * Range:     u8 n <= 140, u16 n <= 5742, u32 n <= 5742, u64 n <= 5742,
 *            u128 n <= 5742, big any n
 * Validated: standard, pass, 81 terms through n = 5587 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

/* number of integer triples (x,y,z) with n = A x^2 + B y^2 + C z^2 */
static uint64_t cg_count(uint64_t n, uint64_t A, uint64_t B, uint64_t C) {
    uint64_t cnt = 0, x, z;
    for (x = 0; A * x * x <= n; x++) {
        uint64_t ax = A * x * x;
        for (z = 0; ax + C * z * z <= n; z++) {
            uint64_t r = n - ax - C * z * z, y, m;
            if (r % B) continue;
            r /= B;
            y = isqrt_u64(r);
            if (y * y != r) continue;
            m = (x ? 2 : 1) * (z ? 2 : 1) * (y ? 2 : 1);
            cnt += m;
        }
    }
    return cnt;
}

static bool cg_congruent_sqfree(uint64_t s) {
    if (s & 1)
        return 2 * cg_count(s, 2, 1, 32) == cg_count(s, 2, 1, 8);
    return 2 * cg_count(s, 8, 2, 64) == cg_count(s, 8, 2, 16);
}

static uint64_t cg_sqfree_part(uint64_t k) {
    uint64_t res = 1, p;
    for (p = 2; p * p <= k; p++) {
        int cnt = 0;
        while (k % p == 0) { k /= p; cnt++; }
        if (cnt & 1) res *= p;
    }
    if (k > 1) res *= k;
    return res;
}

static uint64_t *cg_terms = NULL;
static idx_t cg_len = 0, cg_cap = 0;
static uint64_t cg_next = 1;

seq_t a(idx_t n) {
    while (cg_len < n) {
        uint64_t k = cg_next++;
        if (cg_congruent_sqfree(cg_sqfree_part(k))) {
            if (cg_len == cg_cap) {
                idx_t nc = cg_cap ? cg_cap * 2 : 1024;
                cg_terms = (uint64_t *)realloc(cg_terms, (size_t)nc * sizeof(uint64_t));
                cg_cap = nc;
            }
            cg_terms[cg_len++] = k;
        }
    }
    return (seq_t)cg_terms[n - 1];
}
