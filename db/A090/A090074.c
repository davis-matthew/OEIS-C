/*
 * A090074  (Presumed) last palindrome in the Reverse and Add! trajectory of
 *          10^n.
 * https://oeis.org/A090074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3654563*10^n + 3654563 for n >= 6; base cases: a(0) =
 *            3654563, a(1) = 678736545637876, a(2) = 663305503366, a(3) =
 *            663787366, a(4) = 88352682264077046228625388, a(5) =
 *            365468864563
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 3, u128 n <= 11,
 *            big any n
 * Helpers:   str_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/str_to_seq.h"

seq_t a(idx_t n)
{
    /* base case values as decimal strings (usable in C and C++) */
    static const char * const base_vals[6] = {
        "3654563",
        "678736545637876",
        "663305503366",
        "663787366",
        "88352682264077046228625388",
        "365468864563"
    };

    if (n < 6)
        return str_to_seq(base_vals[n]);

    /* coefficient 3654563 as seq_t */
    static seq_t coeff = 0;
    static int coeff_done = 0;
    if (!coeff_done) {
        coeff = str_to_seq("3654563");
        coeff_done = 1;
    }

    /* compute 10^n in seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= (seq_t)10;

    return coeff * pow10 + coeff;
}
