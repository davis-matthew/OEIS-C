/*
 * A045016  Numbers whose base-4 representation contains no 0's and exactly
 *          three 1's.
 * https://oeis.org/A045016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2561, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045016: Numbers whose base-4 representation contains no 0's
 * and exactly three 1's.
 *
 * This implementation scans successive integers, checks the base‑4
 * digits directly, and returns the nth term (1-indexed).  It uses only
 * integer arithmetic and is valid C11 and C++17.  No memoisation is
 * required; each call computes afresh, which is fast enough for the
 * intended sizes (up to about n = 10000, requiring < 1 M iterations).
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If n == 0 we ignore, as the sequence is 1‑indexed. */
    if (n == 0)
        return 0;

    idx_t found = 0;          /* how many valid numbers have been seen */
    seq_t cand   = 1;          /* current integer to test */

    while (found < n) {
        /* Quick skip: if the least significant base‑4 digit is 0,
         * the number cannot belong to the sequence. */
        if ((cand % 4) != 0) {
            seq_t tmp = cand;
            seq_t count_ones = 0;
            int invalid = 0;

            /* Inspect all base‑4 digits of cand. */
            while (tmp > 0) {
                int d = (int)(tmp % 4);   /* digit in [0,3] */
                if (d == 0) {
                    invalid = 1;
                    break;
                } else if (d == 1) {
                    count_ones++;
                    if (count_ones > 3) {   /* more than three ones */
                        invalid = 1;
                        break;
                    }
                }
                tmp /= 4;
            }

            if (!invalid && count_ones == 3) {
                found++;   /* a valid number has been found */
                if (found == n)
                    return cand;   /* the nth term */
            }
        }
        cand++;   /* move to the next integer */
    }

    /* In practice this point is never reached because the loop
     * returns once the nth term is found.  It is only present to
     * satisfy the compiler that all control paths return a value. */
    return 0;
}
