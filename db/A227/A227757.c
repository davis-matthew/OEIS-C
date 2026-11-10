/*
 * A227757  Numbers k such that antisigma(k) = antisigma(k+1) + 12; where
 *          antisigma(k) = A024816(k).
 * https://oeis.org/A227757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for k with sigma(k+1) - sigma(k) = k + 13 (equivalent to
 *            antisigma(k) = antisigma(k+1) + 12), using a doubling sigma
 *            sieve
 * Range:     u8 n <= 8, u16 n <= 371, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 64 terms through n = 488 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *A227757_terms = 0;
static size_t A227757_cnt = 0, A227757_cap = 0;
static idx_t A227757_limit = 0;

static void A227757_rebuild(idx_t L) {
    uint64_t *sg = (uint64_t *)calloc((size_t)L + 2, sizeof(uint64_t));
    if (!sg) return;
    for (idx_t d = 1; d <= L + 1; ++d)
        for (idx_t m = d; m <= L + 1; m += d)
            sg[m] += d;
    A227757_cnt = 0;
    for (idx_t k = 1; k <= L; ++k) {
        if (sg[k + 1] == sg[k] + (uint64_t)k + 13) {
            if (A227757_cnt == A227757_cap) {
                A227757_cap = A227757_cap ? A227757_cap * 2 : 64;
                A227757_terms = (idx_t *)realloc(A227757_terms, A227757_cap * sizeof(idx_t));
            }
            A227757_terms[A227757_cnt++] = k;
        }
    }
    A227757_limit = L;
    free(sg);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (A227757_cnt < (size_t)n) {
        idx_t L = A227757_limit ? A227757_limit * 4 : 65536;
        A227757_rebuild(L);
    }
    return (seq_t)A227757_terms[n - 1];
}
