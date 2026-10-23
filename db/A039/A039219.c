/*
 * A039219  Numbers whose base-12 representation has the same number of 0's
 *          and 10's.
 * https://oeis.org/A039219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑12 representation contains the same number of
 *            0’s and 10’s. The function computes the n‑th such integer (a(1)
 *            is 1)
 * Range:     u8 n <= 195, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* count how many terms have been seen */
    idx_t seen = 0;

    /* start searching from 1 */
    seq_t cand = (seq_t)1;

    while (1) {
        /* count 0’s and 10’s in the base‑12 representation of cand */
        seq_t tmp = cand;
        size_t zeros = 0, tens = 0;

        while (tmp != (seq_t)0) {
            seq_t digit = tmp % (seq_t)12;
            if (digit == (seq_t)0) {
                ++zeros;
            } else if (digit == (seq_t)10) {
                ++tens;
            }
            tmp = tmp / (seq_t)12;
        }

        if (zeros == tens) {
            ++seen;
            if (seen == n)
                return cand;
        }

        /* advance to the next candidate */
        cand = cand + (seq_t)1;
    }
}
