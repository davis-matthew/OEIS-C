/*
 * A010414  Squares mod 53.
 * https://oeis.org/A010414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence A010414 – squares modulo 53, sorted (quadratic residues mod 53). */
/* The first term a(1) = 0; subsequent terms are the 26 other residues in increasing order. */
/* a(n) = the n‑th smallest integer r ∈ [0,52] for which ∃x: x² ≡ r (mod 53). */

seq_t a(idx_t n)
{
    static seq_t residues[53];
    static size_t residues_len = 0;

    if (!residues_len) {
        unsigned char visited[53] = {0};

        /* compute squares modulo 53 */
        for (idx_t x = 0; x < 53; ++x) {
            seq_t r = (seq_t)x * (seq_t)x % (seq_t)53;
            visited[(unsigned int)r] = 1;
        }

        /* collect the residues in ascending order */
        for (unsigned int r = 0; r < 53U; ++r) {
            if (visited[r]) {
                residues[residues_len++] = (seq_t)r;
            }
        }
    }

    if (n == 0 || (size_t)n > residues_len) {
        /* Undefined for out‑of‑range indices – return 0 as a safe placeholder. */
        return 0;
    }

    return residues[(size_t)(n - 1)];
}
