/*
 * A045253  Numbers whose base-5 representation contains exactly one 1 and two
 *          4's.
 * https://oeis.org/A045253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑5 representation contains exactly one 1 and two 4's;
   computed by iterating over increasing numbers and counting matches. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)0;
    idx_t count = (idx_t)0;
    while (true) {
        val += (seq_t)1;                 /* next candidate number */
        seq_t tmp = val;
        int c1 = 0, c4 = 0;
        while (tmp > (seq_t)0) {         /* examine base‑5 digits */
            seq_t digit = tmp % (seq_t)5;
            if (digit == (seq_t)1) {
                ++c1;
                if (c1 > 1) break;      /* more than one 1 – reject */
            } else if (digit == (seq_t)4) {
                ++c4;
                if (c4 > 2) break;      /* more than two 4s – reject */
            }
            tmp /= (seq_t)5;
        }
        if (c1 == 1 && c4 == 2) {        /* exactly one 1 and two 4s */
            ++count;
            if (count == n) return val;  /* desired term found */
        }
    }
}
