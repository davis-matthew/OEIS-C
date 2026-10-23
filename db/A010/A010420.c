/*
 * A010420  Squares mod 59.
 * https://oeis.org/A010420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth nonnegative integer whose residue modulo 59 is a
 *            quadratic residue
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#define PRIME_MOD 59
enum { NUM_RESIDUES = 30 };   /* (PRIME_MOD + 1)/2 */

seq_t a(idx_t n)
{
    static seq_t residues[NUM_RESIDUES];
    static bool initialized = false;

    if (!initialized) {
        bool seen[PRIME_MOD] = { false };
        idx_t r, i = 0;

        /* collect the unique quadratic residues modulo 59 */
        for (r = 0; r < PRIME_MOD; ++r) {
            seq_t sq = ((seq_t)r * (seq_t)r) % PRIME_MOD;
            if (!seen[(int)sq]) {
                seen[(int)sq] = true;
                residues[i++] = sq;
            }
        }

        /* sort residues in ascending order (insertion sort, cheap for 30 elements) */
        for (idx_t j = 1; j < NUM_RESIDUES; ++j) {
            seq_t key = residues[j];
            idx_t k = j;
            while (k > 0 && residues[k - 1] > key) {
                residues[k] = residues[k - 1];
                --k;
            }
            residues[k] = key;
        }
        initialized = true;
    }

    idx_t idx   = n - 1;         /* zero‑based index into the infinite sequence */
    idx_t block = idx / NUM_RESIDUES;  /* which 59‑block (each block contains NUM_RESIDUES items) */
    idx_t pos   = idx % NUM_RESIDUES;  /* position within that block */

    seq_t offset = (seq_t)block * PRIME_MOD;  /* add 59 for each block */
    return residues[pos] + offset;
}
