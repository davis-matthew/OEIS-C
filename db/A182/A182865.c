/*
 * A182865  Minimal number of quadratic residues: a(n) is the least integer m
 *          such that any nonzero square is congruent (mod n) to one of the
 *          squares from 1 to m^2.
 * https://oeis.org/A182865
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 328, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) of OEIS A182865: the minimal integer m such that any
   non‑zero quadratic residue modulo n is congruent to one of the squares
   1², 2², …, m².
   The function ignores the residue 0, which is not counted in the
   definition of the sequence.
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Index n must be ≥ 2 for the sequence; return 0 for smaller values. */
    if (n < 2u) return (seq_t)0;

    /* Allocate arrays to keep track of seen residues and covered residues.
       Size n is sufficient because residues are in the range [0, n‑1]. */
    size_t sz = (size_t)n;
    unsigned char *seen   = (unsigned char *)malloc(sz);
    unsigned char *covered = (unsigned char *)malloc(sz);
    if (!seen || !covered) {
        free(seen);
        free(covered);
        return (seq_t)0;
    }
    for (size_t i = 0; i < sz; ++i) {
        seen[i] = 0;
        covered[i] = 0;
    }

    /* Count distinct non‑zero quadratic residues modulo n. */
    size_t resid_count = 0;
    for (idx_t x = 1; x < n; ++x) {
        seq_t r = ((seq_t)x * (seq_t)x) % (seq_t)n;
        if (r == 0) continue;            /* ignore residue 0 */
        if (!seen[(size_t)r]) {
            seen[(size_t)r] = 1;
            ++resid_count;
        }
    }

    /* Find smallest m so that squares 1²…m² cover all non‑zero residues. */
    size_t cover_cnt = 0;
    for (idx_t m = 1; ; ++m) {
        seq_t r = ((seq_t)m * (seq_t)m) % (seq_t)n;
        if (r == 0) continue;            /* 0 is not required to be covered */
        if (!covered[(size_t)r]) {
            covered[(size_t)r] = 1;
            ++cover_cnt;
            if (cover_cnt == resid_count) {
                free(seen);
                free(covered);
                return (seq_t)m;
            }
        }
    }
}
