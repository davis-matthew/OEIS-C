/*
 * A190246  Positions of 0 in A190245; complement of A190247.
 * https://oeis.org/A190246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 3354, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth integer (each ≥ 1) for which
   floor(3*sqrt(2)*n) = floor(sqrt(2)*n) + floor(2*sqrt(2)*n).
   The computation uses only integer arithmetic and the helper
   floor_sqrt(seq_t) to evaluate floor(sqrt(k*n^2)). */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Verify that n is positive; we assume caller passes n ≥ 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t i = 1;              /* Candidate integer n (index in OEIS) */
    idx_t cnt = 0;            /* Number of suitable integers found so far */

    for (;;) {
        /* Compute floor(sqrt(2)*i), floor(sqrt(8)*i), floor(sqrt(18)*i)
           using floor_sqrt on 2*i^2, 8*i^2, and 18*i^2. */
        seq_t sq = (seq_t)i * (seq_t)i;          /* i^2 */
        seq_t a = floor_sqrt( (seq_t)2 * sq );   /* floor(√(2) * i) */
        seq_t b = floor_sqrt( (seq_t)8 * sq );   /* floor(√(8) * i) */
        seq_t c = floor_sqrt( (seq_t)18 * sq );  /* floor(√(18) * i) */

        if (c == a + b) {
            cnt++;
            if (cnt == n) {
                return (seq_t)i;
            }
        }
        i++;
    }
}
