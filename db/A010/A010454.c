/*
 * A010454  Squares mod 93.
 * https://oeis.org/A010454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010454 (Squares mod 93):
   a(n) is the n-th smallest residue r (0 ≤ r < 93) such that
   there exists an integer x with x² ≡ r (mod 93).  The residues
   are listed in ascending order.  This implementation precomputes
   the full set of residues once and returns the n‑th element. */

#include <stdbool.h>

static bool    initialized = false;
static seq_t   residues[93];
static idx_t   count_residues = 0;

static void
init_residues(void)
{
    /* mark all residues that appear as a square modulo 93 */
    bool marked[93] = { false };
    for (idx_t i = 0; i < 93; ++i) {
        seq_t r = ( (seq_t)i * (seq_t)i ) % 93;
        marked[(int)r] = true;
    }
    /* collect them in ascending order */
    for (idx_t r = 0; r < 93; ++r) {
        if (marked[(int)r]) {
            residues[count_residues++] = r;
        }
    }
    initialized = true;
}

seq_t
a(idx_t n)
{
    if (!initialized) {
        init_residues();
    }
    if (n == 0 || n > count_residues) {
        /* The sequence is defined only up to the number of distinct quadratic
           residues modulo 93 (32).  For out‑of‑range indices this implementation
           returns 0, but callers should not request such indices. */
        return 0;
    }
    return residues[n - 1];
}
