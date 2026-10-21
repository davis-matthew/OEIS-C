/*
 * A095028  An example of a (v,k,lambda)=(11,5,2) cyclic difference set.
 * https://oeis.org/A095028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the nth non‑zero quadratic residue modulo 11, in
 *            ascending order. Therefore a(1)=1, a(2)=3, a(3)=4, a(4)=5,
 *            a(5)=9 and a(n)=0 for n>5
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool init_done = false;
static seq_t residues[5];

static void init_residues(void) {
    if (init_done) return;
    /* Compute non‑zero quadratic residues modulo 11 and sort them */
    int v = 11;
    int k = 5;
    bool seen[11] = {0};
    for (int x = 1; x < v; ++x) {
        int r = (x * x) % v;
        if (r != 0) seen[r] = true;
    }
    int idx = 0;
    for (int r = 1; r < v; ++r) {
        if (seen[r]) residues[idx++] = (seq_t)r;
    }
    init_done = true;
}

seq_t a(idx_t n) {
    init_residues();
    if (n >= 1 && n <= 5) {
        return residues[n - 1];
    }
    return 0; /* undefined terms are defined as 0 */
}
