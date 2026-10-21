/*
 * A055008  Numbers k such that gcd(phi(k), sigma(k)) = 1 with phi = A000010,
 *          sigma = A000203.
 * https://oeis.org/A055008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055008: numbers k such that gcd(phi(k), sigma(k)) = 1. */
/* The sequence consists only of squares and twice squares, so we generate
   candidates in increasing order by merging these two monotone lists. */
/* A candidate k is accepted if gcd(phi(k), sigma(k)) == 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static storage for the generated sequence */
    static seq_t *seq = NULL;
    static size_t seq_size = 0;
    static size_t seq_cap  = 0;

    /* maintain the next square and twice‑square factors */
    static idx_t sq_s = 1;  /* for squares: k = sq_s^2   */
    static idx_t sq_t = 1;  /* for twice squares: k = 2*sq_t^2 */

    /* generate terms until we have enough */
    while (seq_size < n) {
        seq_t cand_sq  = (seq_t)sq_s * sq_s;
        seq_t cand_tw  = (seq_t)2 * sq_t * sq_t;
        seq_t cand = (cand_sq <= cand_tw) ? cand_sq : cand_tw;

        /* test the condition */
        seq_t phi_k = phi((idx_t)cand);
        seq_t sigma_k = sigma_of((idx_t)cand);
        if (gcd(phi_k, sigma_k) == (seq_t)1) {
            /* store the candidate */
            ensure_capacity(seq_size + 1, &seq_cap, (void**)&seq, sizeof(seq_t));
            seq[seq_size++] = cand;
        }

        /* advance the list that contributed the chosen candidate */
        if (cand == cand_sq)
            sq_s++;
        else
            sq_t++;
    }

    return seq[n - 1];
}
