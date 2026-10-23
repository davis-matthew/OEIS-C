/*
 * A045163  Numbers whose base-5 representation contains exactly one 0 and two
 *          1's.
 * https://oeis.org/A045163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose base‑5 representation contains exactly
   one 0 and two 1's.  If the sequence were defined by an explicit formula
   this comment would state that formula. */

#include <stdbool.h>

/* Return true if x has exactly one digit 0 and two digits 1 in base 5. */
static bool has_property(seq_t x)
{
    idx_t zeros = 0, ones = 0;
    while (x > 0) {
        int d = (int)(x % 5);
        if (d == 0) zeros++;
        else if (d == 1) ones++;
        x /= 5;
    }
    return zeros == 1 && ones == 2;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;

    while (found < n) {
        candidate++;                 /* move to next integer          */
        if (has_property(candidate)) {
            found++;                  /* a valid term found            */
        }
    }
    return candidate;                 /* candidate is the nth term      */
}
