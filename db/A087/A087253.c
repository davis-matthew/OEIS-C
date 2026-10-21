/*
 * A087253  Number of distinct initial values of various 3x+1 trajectories of
 *          which the peak-value equals 4n.
 * https://oeis.org/A087253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A087253: counts the number of starting integers iv such that the
   maximum value reached in the Collatz (3x+1) trajectory starting from iv
   equals 4*n. The function enumerates all iv from 1 to 4*n,
   follows their trajectories until it either reaches 1 or a value larger
   than 4*n (in which case the trajectory cannot satisfy the condition).
   The running time is O(n) with a very small constant factor. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t peak = ((seq_t)n) * (seq_t)4;      /* maximum value to look for */
    seq_t count = 0;                         /* result accumulator */

    /* Iterate over all possible starting values up to the peak */
    for (idx_t start = 1; start <= (idx_t)peak; ++start) {
        seq_t v = start;                     /* current value in trajectory */
        seq_t localMax = v;                  /* track maximum reached so far */
        bool exceeds = false;                /* flag if max > peak */

        /* Follow the Collatz trajectory */
        while (v != 1) {
            /* If we have already exceeded the peak, this starting number
               cannot have max exactly equal to peak. */
            if (v > peak) {
                exceeds = true;
                break;
            }

            /* Advance to the next value in the trajectory */
            if (v & 1) {                      /* odd */
                v = v * (seq_t)3 + (seq_t)1;
            } else {                          /* even */
                v >>= 1;
            }

            /* Update local maximum */
            if (v > localMax) {
                localMax = v;
            }
        }

        /* Count the starting value only if the maximum equals the peak
           and no value exceeded the peak during the trajectory. */
        if (!exceeds && localMax == peak) {
            ++count;
        }
    }

    return count;
}
