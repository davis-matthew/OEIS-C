/*
 * A010456  Squares mod 95.
 * https://oeis.org/A010456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = r_k + 95*m, where r_k is the k‑th quadratic residue
 *            modulo 95 sorted increasingly, k = (n-1) mod 30, m = (n-1)/30
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t residues[30] = {0};
static bool residues_initialized = false;

static void compute_residues(void)
{
    bool seen[95] = { false };
    for (idx_t i = 0; i < 95; ++i) {
        seq_t r = (seq_t)i * (seq_t)i % (seq_t)95;
        seen[(int)r] = true;
    }
    idx_t pos = 0;
    for (idx_t r = 0; r < 95; ++r) {
        if (seen[(int)r]) {
            residues[pos++] = (seq_t)r;
        }
    }
    residues_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, keep 0 */
    if (!residues_initialized) compute_residues();

    idx_t idx   = (n - 1) % 30;            /* residue index   */
    idx_t block = (n - 1) / 30;            /* number of full 95‑blocks */

    return residues[idx] + (seq_t)block * (seq_t)95;
}
