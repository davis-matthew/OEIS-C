/*
 * A043862  Number of runs in the base 4 representation of n is congruent to 4
 *          mod 9.
 * https://oeis.org/A043862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose base‑4 representation has a number of
 *            runs congruent to 4 modulo 9. (Runs are maximal contiguous
 *            blocks of identical digits.)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* should not happen – safety */

    seq_t num = 0;                  /* current integer to test   */
    idx_t  found = 0;               /* how many qualifying values seen */

    while (found < n) {
        num++;                      /* test next integer         */

        seq_t tmp   = num;          /* copy for base‑4 reduction */
        seq_t runs  = 0;
        int    prev  = -1;          /* digit before the current -- -1 ≠ any base‑4 digit */

        while (tmp > 0) {           /* count the runs in base‑4  */
            int d = (int)(tmp % 4); /* digit 0..3                */
            if (d != prev) {        /* new run starts            */
                runs++;
                prev = d;
            }
            tmp /= 4;               /* move to next digit        */
        }

        if (((int)runs % 9) == 4)   /* desired congruence        */
            found++;
    }

    return num;                     /* num is the n‑th qualifying integer */
}
