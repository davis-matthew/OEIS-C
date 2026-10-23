/*
 * A166040  Number of times Sum_{i=1..u} J(i,2n+1) obtains value zero when u
 *          ranges from 1 to (2n+1). Here J(i,k) is the Jacobi symbol.
 * https://oeis.org/A166040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 9865, u32 n <= 9865, u64 n <= 9865,
 *            u128 n <= 9865, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A166040: the number of times the partial sum of
   Jacobi symbols J(i,2n+1) equals zero for 1 ≤ i ≤ 2n.  */
#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* k = 2n + 1, the odd modulus in the sequence definition */
    uint64_t k = 2ULL * (uint64_t)n + 1ULL;

    /* Sum of Jacobi symbols; it may become negative, so use a signed type. */
    int64_t sum = 0;

    seq_t zeros = 0;  /* count of u with partial sum zero (excluding u = 2n+1) */

    /* Iterate i = 1 … k-1  (i = 2n+1 gives J = 0 and does not change the sum) */
    for (uint64_t i = 1; i < k; ++i)
    {
        int j = jacobi_symbol(i, k);  /* J(i,k) ∈ { -1, 0, 1 } */

        /* Update the running sum */
        sum += j;     /* j is -1, 0 or 1, int64_t handles it safely */

        /* Count the zero when reached */
        if (sum == 0)
            ++zeros;
    }

    return zeros;
}
