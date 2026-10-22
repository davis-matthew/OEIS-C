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
 * Helpers:   cg_congruent_sqfree, cg_count, cg_isqrt, cg_sqfree_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cg_congruent_sqfree.h"
#include "../utils/cg_count.h"
#include "../utils/cg_isqrt.h"
#include "../utils/cg_sqfree_part.h"

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
