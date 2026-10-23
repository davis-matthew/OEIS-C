/*
 * A010444  Squares mod 83.
 * https://oeis.org/A010444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Quadratic residues modulo 83, sorted in ascending order. */
/* The sequence is a(n) = the n‑th smallest residue of k^2 (mod 83) for k = 0,…,82,
   with the offset 1 (a(1)=0). The values repeat every 42 terms. */

#include <stddef.h>
#include <stdbool.h>

static bool ready = false;
static seq_t residues[42];
static size_t residue_count;

static void init_residues(void)
{
    bool seen[83] = {false};
    /* Mark each quadratic residue modulo 83. */
    for (idx_t i = 0; i < 83; ++i) {
        idx_t sq = (i * i) % 83;  /* i is <= 82, no overflow */
        seen[sq] = true;
    }
    /* Collect the residues in sorted order. */
    size_t pos = 0;
    for (idx_t r = 0; r < 83; ++r) {
        if (seen[r]) {
            residues[pos++] = (seq_t)r;
        }
    }
    residue_count = pos;   /* should be 42 */
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) {
        init_residues();
    }
    /* n >= 1 according to the OEIS offset; wrap around after 42 values. */
    idx_t idx = (n - 1) % (idx_t)residue_count;
    return residues[idx];
}
