/*
 * A010437  Squares mod 76.
 * https://oeis.org/A010437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Quadratic residues modulo 76, sorted ascending
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t a(idx_t n) returns the n-th smallest residue r such that r ≡ k² (mod 76). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* seq_t and idx_t are supplied by the build environment */

static bool initialized = false;           /* initialisation flag */
static seq_t residues[76];                 /* sorted residues */
static size_t residues_count = 0;          /* number of distinct residues */

seq_t a(idx_t n)
{
    if (!initialized) {
        /* compute all quadratic residues modulo 76 */
        bool seen[76] = {0};
        for (idx_t k = 0; k < 76; ++k) {
            uint64_t sq = (uint64_t)k * (uint64_t)k;
            uint64_t mod = sq % 76;
            seen[(int)mod] = true;
        }
        /* collect residues in ascending order */
        size_t pos = 0;
        for (int v = 0; v < 76; ++v) {
            if (seen[v]) {
                residues[pos++] = (seq_t)v;
            }
        }
        residues_count = pos;
        initialized = true;
    }

    if (n < 1 || (size_t)n > residues_count) {
        return 0;   /* undefined for n outside the finite sequence */
    }
    return residues[(size_t)(n - 1)];
}
