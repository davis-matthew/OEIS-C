/*
 * A095780  Values of n for which A095777(n) is 11 (those terms which are
 *          expressible in decimal digits for bases 2 through 12, but not for
 *          base 13).
 * https://oeis.org/A095780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer that is representable in every
 *            base b with 2 ≤ b ≤ 12 using only decimal digits 0–9, but fails
 *            (has a digit ≥ 10) in base 13
 * Range:     u8 n <= 52, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   all_dec_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/all_dec_digits.h"

seq_t a(idx_t n)
{
    seq_t cand = 1;
    idx_t found = 0;

    for (;; ++cand) {
        /* check bases 2..12 */
        int b;
        for (b = 2; b <= 12; ++b) {
            if (!all_dec_digits(cand, b)) break;
        }
        if (b <= 12) continue;          /* failed before base 12 */

        /* base 13 must contain a non‑decimal digit */
        if (all_dec_digits(cand, 13)) continue;   /* base 13 succeeded → not a term */

        ++found;
        if (found == n) return cand;
    }
}
