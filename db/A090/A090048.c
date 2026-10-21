/*
 * A090048  Length of longest contiguous block of 0's in binary expansion of
 *          n-th triangular number.
 * https://oeis.org/A090048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest contiguous block of 0's in the binary
 *            expansion of the n-th triangular number T(n) = n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 2064, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute T(n) as seq_t to avoid overflow */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Special case: T(0) = 0 has a single 0 bit */
    if (t == 0) {
        return 1;
    }

    seq_t max_run = 0;   /* longest run of zeros seen so far */
    seq_t run     = 0;   /* current run of zeros */
    seq_t tmp     = t;   /* iterate over bits without altering t */

    while (tmp != 0) {
        /* Check the least significant bit */
        if ((tmp & 1) == 0) {
            /* zero continues or starts a run */
            run++;
            if (run > max_run) {
                max_run = run;
            }
        } else {
            /* hit a 1, reset current run */
            run = 0;
        }
        tmp >>= 1;  /* shift to process the next bit */
    }

    return max_run;
}
