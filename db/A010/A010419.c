/*
 * A010419  Squares mod 58.
 * https://oeis.org/A010419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th smallest integer k such that there exists an
 *            integer x with x² ≡ k (mod 58). In other words, the sorted list
 *            of distinct quadratic residues modulo 58
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t residues[58];
static size_t residue_count = 0;
static bool initialized = false;

static void init_residues(void)
{
    bool seen[58] = { false };
    for (idx_t k = 0; k < 58; ++k) {
        idx_t r = (idx_t)(((uint64_t)k * (uint64_t)k) % 58U);
        seen[(size_t)r] = true;
    }
    for (idx_t v = 0; v < 58; ++v) {
        if (seen[(size_t)v]) {
            residues[residue_count++] = (seq_t)v;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_residues();
    }
    if (n == 0 || (size_t)n > residue_count) {
        /* Sequence is defined only for 1 ≤ n ≤ residue_count. */
        return (seq_t)0;
    }
    return residues[(size_t)(n - 1)];
}
