/*
 * A127834  Numbers whose 8-bit binary representation, when rotated by up to
 *          one bit, contains every 3-bit binary representation for the
 *          numbers 0 through 7.
 * https://oeis.org/A127834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_debruijn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sorted residues of 8‑bit de Bruijn sequences + 256·floor((n‑1)/16) */
/* The residues are computed on the first call by testing all 256 8‑bit values.
   Only 16 such residues exist.  All further terms are just these residues
   repeated every 256 steps. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_debruijn.h"

static seq_t residues[16];
static size_t residue_count = 0;
static bool residues_initialized = false;

/* Sort the residues array in ascending order (insertion sort, tiny size). */
static void sort_residues(void)
{
    for (size_t i = 1; i < residue_count; ++i) {
        seq_t key = residues[i];
        size_t j = i;
        while (j > 0 && residues[j - 1] > key) {
            residues[j] = residues[j - 1];
            --j;
        }
        residues[j] = key;
    }
}

void init_residues(void)
{
    if (residues_initialized) return;
    /* Test all 256 possible 8‑bit values */
    for (seq_t x = 0; x < 256; ++x) {
        if (is_debruijn(x)) {
            residues[residue_count++] = x;
            if (residue_count >= 16) break;  /* safety */
        }
    }
    sort_residues();
    residues_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_residues();             /* ensures residues[0..15] are ready */

    /* 0‑based index offset */
    idx_t k = n - 1;
    idx_t q = k / 16;            /* number of full residue blocks */
    idx_t r = k % 16;            /* index within the block */

    /* Compute result: residue + 256 * q */
    seq_t base  = residues[r];
    seq_t add   = (seq_t)q * ((seq_t)256);
    return base + add;
}
