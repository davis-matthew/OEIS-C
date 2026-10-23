/*
 * A037938  Trajectory of 42 under prime factor concatenation procedure.
 * https://oeis.org/A037938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 42 under prime factor concatenation procedure.
   a(0) = 42
   a(1) = 2
   a(2) = 3
   a(3) = 7
   a(4) = 3
   a(5) = 79
   a(6) = 379
   For all n ≥ 6 the sequence stabilises at 379. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First seven terms are known explicitly. */
    static const seq_t first[7] = {42, 2, 3, 7, 3, 79, 379};

    if (n < 7)
        return first[(size_t)n];

    /* After the transient the composite 379 is prime and
       consequently the trajectory stays constant. */
    return 379;
}
