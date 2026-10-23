/*
 * A010424  Squares mod 63.
 * https://oeis.org/A010424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 63 · ⌊(n-1)/16⌋ + r[(n-1) mod 16] * where r[0..15] is the
 *            sorted list of quadratic residues modulo 63 *
 *            (0,1,4,7,9,16,18,22,25,28,36,37,43,46,49,58)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int MOD = 63;
static seq_t residues[16];
static bool residues_initialized = false;

static void init_residues(void)
{
    bool seen[MOD];
    for (int i = 0; i < MOD; ++i) seen[i] = false;

    for (idx_t i = 0; i < (idx_t)MOD; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i % (seq_t)MOD;
        seen[(int)sq] = true;
    }

    int count = 0;
    for (int r = 0; r < MOD; ++r) {
        if (seen[r]) {
            residues[count++] = (seq_t)r;
        }
    }
}

seq_t a(idx_t n)
{
    if (!residues_initialized) {
        init_residues();
        residues_initialized = true;
    }

    idx_t idx   = n - 1;
    idx_t block = idx / 16;
    idx_t pos   = idx % 16;

    return (seq_t)block * (seq_t)MOD + residues[(int)pos];
}
