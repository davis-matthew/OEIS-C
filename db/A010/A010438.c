/*
 * A010438  Squares mod 77.
 * https://oeis.org/A010438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth non‑negative integer that is a quadratic residue
 *            * modulo 77, i.e. there exists an integer x with x^2 ≡ a(n) (mod
 *            77)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute the 24 quadratic residues modulo 77 once. */
    static bool initialized = false;
    static bool is_residue[77];
    static const idx_t RES_COUNT = 24;          /* number of residues modulo 77 */

    if (!initialized) {
        /* Mark all quadratic residues. */
        for (idx_t i = 0; i < 77; ++i)
            is_residue[(i * i) % 77] = true;
        initialized = true;
    }

    /* For n ≥ 1: calculate block number k and residue index r. */
    idx_t k = (n - 1) / RES_COUNT;   /* how many full blocks of 77 */
    idx_t r = (n - 1) % RES_COUNT;   /* position within the current block */

    /* Find the r‑th smallest residue. */
    idx_t pos = 0;
    for (idx_t val = 0; val < 77; ++val) {
        if (is_residue[val]) {
            if (pos == r)
                return (seq_t)val + (seq_t)k * 77;
            ++pos;
        }
    }

    /* The code should never reach here for n ≥ 1. */
    return 0;
}
