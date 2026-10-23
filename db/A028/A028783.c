/*
 * A028783  Nonsquares mod 70.
 * https://oeis.org/A028783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th positive integer that is NOT a quadratic
 *            residue modulo 70
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028783: Nonsquares mod 70.    */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* lazily initialise the set of residues modulo 70 */
    static bool initialized = false;
    static unsigned char nonres[70];  /* residues that are NOT quadratic residues */
    static idx_t nonres_count = 0;

    if (!initialized) {
        bool used[70] = {false};
        for (uint64_t k = 0; k < 70; ++k) {
            uint64_t r = (k * k) % 70;
            used[r] = true;
        }
        idx_t cnt = 0;
        for (uint64_t i = 0; i < 70; ++i) {
            if (!used[i]) {
                nonres[cnt++] = (unsigned char)i;
            }
        }
        nonres_count = cnt;
        initialized = true;
    }

    idx_t block   = (n - 1) / nonres_count;
    idx_t residue = (n - 1) % nonres_count;
    seq_t val = (seq_t)block * (seq_t)70 + nonres[residue];
    return val;
}
