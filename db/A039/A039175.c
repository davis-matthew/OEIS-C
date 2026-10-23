/*
 * A039175  Numbers whose base-11 representation has the same number of 2's
 *          and 4's.
 * https://oeis.org/A039175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039175: numbers whose base‑11 representation has equal
   number of 2's and 4's
   The function computes each term by brute‑force testing of successive
   integers. */
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t found = 0;            /* how many qualifying numbers found */
    seq_t num = 0;            /* current integer under examination */

    for (;;) {
        /* Count digits 2 and 4 in the base‑11 representation of num. */
        unsigned cnt2 = 0, cnt4 = 0;
        seq_t tmp = num;

        while (tmp != 0) {
            uint64_t d = (uint64_t)(tmp % 11);    /* digit in [0,10]    */
            if (d == 2) cnt2++;
            else if (d == 4) cnt4++;
            tmp /= 11;
        }

        if (cnt2 == cnt4) {
            found++;
            if (found == n) {
                return num;   /* n‑th term found */
            }
        }
        num++;                  /* move to next integer */
    }
}
