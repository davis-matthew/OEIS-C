/*
 * A110839  Weight enumerator of [16,15,2] even weight code (the Reed-Muller
 *          code RM(3,4)).
 * https://oeis.org/A110839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = binom(16,2*n)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n‑th term of the weight enumerator of the [16,15,2] even
   weight code (Reed-Muller RM(3,4)) sequence.
   a(n) = C(16,2*n) for n = 0 .. 8, and a(n) = 0 for n > 8. */
#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* If n corresponds to a weight beyond 16 (i.e., 2*n > 16) then
       the coefficient is zero. */
    if (n > 8u)
        return 0;

    /* Compute C(16, 2*n) directly. Since n <= 8, 2*n fits safely in
       idx_t and binom_small implements the multiplicative method. */
    idx_t k = 2 * n;
    return binom_small(16, k);
}
