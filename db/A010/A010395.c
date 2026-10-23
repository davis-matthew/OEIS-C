/*
 * A010395  Squares mod 34.
 * https://oeis.org/A010395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th smallest quadratic residue modulo 34
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

enum { RES_COUNT = 18 };

static seq_t residues[RES_COUNT];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        idx_t count = 0;
        for (idx_t k = 0; k < 34; ++k) {
            seq_t sq = (seq_t)k * (seq_t)k;
            seq_t r  = sq % (seq_t)34;
            /* check if r already in residues */
            bool seen = false;
            for (idx_t j = 0; j < count; ++j) {
                if (residues[j] == r) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                residues[count++] = r;
            }
        }
        /* sort residues ascending */
        for (idx_t i = 0; i < RES_COUNT - 1; ++i) {
            idx_t min_idx = i;
            for (idx_t j = i + 1; j < RES_COUNT; ++j) {
                if (residues[j] < residues[min_idx]) {
                    min_idx = j;
                }
            }
            if (min_idx != i) {
                seq_t tmp = residues[i];
                residues[i] = residues[min_idx];
                residues[min_idx] = tmp;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > RES_COUNT) {
        return (seq_t)0;
    }

    return residues[n - 1];
}
