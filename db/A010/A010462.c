/*
 * A010462  Squares mod 30.
 * https://oeis.org/A010462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squares modulo 30 (integers whose residue modulo 30 is a quadratic residue). */
/* The period is 30, with the 12 residues 0,1,4,6,9,10,15,16,19,21,24,25 in ascending order. */
/* For n >= 1: a(n) = 30 * ((n-1)/12) + residues[(n-1)%12] */

#include <stdbool.h>

static seq_t residues[12];
static bool residues_computed = false;

static void compute_residues(void)
{
    bool seen[30] = { false };
    int cnt = 0;
    for (idx_t i = 0; i < 30; ++i) {
        seq_t sq = ((seq_t)i * (seq_t)i) % (seq_t)30;
        if (!seen[(int)sq]) {
            seen[(int)sq] = true;
            residues[cnt++] = sq;
        }
    }
    /* sort residues ascending (simple insertion sort, at most 12 elements) */
    for (int i = 0; i < cnt; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            if (residues[j] < residues[i]) {
                seq_t tmp = residues[i];
                residues[i] = residues[j];
                residues[j] = tmp;
            }
        }
    }
    residues_computed = true;
}

seq_t a(idx_t n)
{
    if (!residues_computed)
        compute_residues();

    idx_t idx   = n - 1;
    idx_t block = idx / 12;
    idx_t pos   = idx % 12;
    seq_t add   = (seq_t)(block * (idx_t)30);
    seq_t result = residues[pos] + add;
    return result;
}
