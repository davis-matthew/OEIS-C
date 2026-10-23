/*
 * A159715  Number of permutations of 2 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*3^(n-2) (for n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Return 0 for n < 2; the OEIS offset is 2, so behaviour below 2 is irrelevant. */
    if (n < 2)
        return 0;

    /* Compute 3^(n-2) using the utility function pow3. */
    seq_t pow_term = pow3((idx_t)(n - 2));

    /* Compute 2*n as a seq_t value. */
    seq_t two_n = ((seq_t)n) * ((seq_t)2);

    /* Final result: 2*n * 3^(n-2) */
    return pow_term * two_n;
}
