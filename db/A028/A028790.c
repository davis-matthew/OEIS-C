/*
 * A028790  Nonsquares mod 77.
 * https://oeis.org/A028790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: full, pass, 53 terms through n = 53 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-square residues modulo 77: return the nth positive integer that is not a
   quadratic residue modulo 77. */

#include <stdbool.h>

/* Utility: check whether x is a quadratic residue mod 77 */
static bool is_residue_mod77(idx_t x)
{
    /* Squares modulo 7: 0,1,2,4 */
    static const bool good7[7] = {true, true, true, false, true, false, false};
    /* Squares modulo 11: 0,1,3,4,5,9 */
    static const bool good11[11] = {true, true, false, true, true, true,
                                    false, false, false, true, false};

    idx_t r7 = x % 7ULL;
    idx_t r11 = x % 11ULL;

    return good7[r7] && good11[r11];
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    for (;;) {
        if (!is_residue_mod77(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;
    }
}
