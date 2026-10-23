/*
 * A028778  Nonsquares mod 65.
 * https://oeis.org/A028778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that are NOT quadratic residues modulo 65.
   a(n) = the n-th positive integer x such that x is not a quadratic
   residue modulo 65. This is equivalent to listing all residues
   modulo 65 that are not square residues modulo 65, then repeating
   every period of 65. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cand = 1;
    idx_t cnt  = 0;

    for (;;)
    {
        /* advance to next candidate */
        cand++;

        /* compute residues modulo 5 and 13 */
        seq_t r5  = cand % 5;
        seq_t r13 = cand % 13;

        /* check if cand is a quadratic residue modulo 5 */
        bool is_r5 = (r5 == 0) || (r5 == 1) || (r5 == 4);

        /* check if cand is a quadratic residue modulo 13 */
        bool is_r13 = (r13 == 0) || (r13 == 1) || (r13 == 3) ||
                      (r13 == 4) || (r13 == 9) || (r13 == 10) ||
                      (r13 == 12);

        /* if it is not a quadratic residue modulo 65, count it */
        if (!(is_r5 && is_r13))
        {
            cnt++;
            if (cnt == n)
                return cand;
        }
    }
}
