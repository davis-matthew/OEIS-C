/*
 * A039160  Numbers whose base-11 representation has the same number of 0's
 *          and 5's.
 * https://oeis.org/A039160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039160: numbers whose base‑11 representation has the same number of 0's and 5's
   Implementation: brute‑force search for the nth term.
   The function works for any seq_t type that supports +, -, *, /, %, comparisons, and the
   literals 0, 5 and 11. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t      found = 0;          /* number of terms found so far          */
    seq_t      value = 1;          /* current candidate (starts from 1)     */

    while (1) {
        seq_t tmp = value;
        unsigned zeros = 0, fives = 0;

        /* Count digits 0 and 5 in the base‑11 representation of tmp. */
        if (tmp == 0) {
            zeros = 1;   /* only useful for value==0, not used here          */
        } else {
            while (tmp > 0) {
                seq_t digit = tmp % 11;   /* base‑11 digit */
                if (digit == 0)          zeros++;
                else if (digit == 5)     fives++;
                tmp /= 11;
            }
        }

        /* If the condition holds, we have found a term of the sequence. */
        if (zeros == fives) {
            found++;
            if (found == n) return value;
        }

        /* Advance to the next integer. */
        value++;
    }
}
