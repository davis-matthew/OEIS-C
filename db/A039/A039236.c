/*
 * A039236  Numbers whose base-12 representation has the same number of 2's
 *          and 9's.
 * https://oeis.org/A039236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   equal_two_nine
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039236: Numbers whose base‑12 representation has equal numbers of the digits 2 and 9,
   computed by a simple brute‑force scan. */
#include <stdint.h>
#include "../utils/equal_two_nine.h"

/* Return the nth term of the sequence (a(1) = 0, a(2) = 1, …). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;              /* undefined for n=0, but safe value */

    seq_t count = (seq_t)0;
    seq_t v     = (seq_t)0;

    while (1) {
        if (equal_two_nine(v)) {
            count++;
            if (count == (seq_t)n)
                return v;
        }
        v = v + (seq_t)1;             /* move to next integer */
    }
}
