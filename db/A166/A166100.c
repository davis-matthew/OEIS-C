/*
 * A166100  Sum of those positive i <= 2n+1, for which J(i,2n+1)=+1. Here
 *          J(i,k) is the Jacobi symbol.
 * https://oeis.org/A166100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 259, u32 n <= 8749, u64 n <= 8749,
 *            u128 n <= 8749, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of all 1 ≤ i ≤ 2n+1 with Jacobi symbol J(i,2n+1) = +1 */
#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    idx_t odd = (idx_t)2 * n + 1;               /* odd = 2n+1 */
    if (odd == 1) {                            /* special case: modulus 1 */
        /* By convention J(i,1) = 1 for every i, so the sum is just i=1 */
        return (seq_t)1;
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= odd; ++i) {
        if (jacobi_symbol((uint64_t)i, odd) == 1) {
            sum += (seq_t)i;
        }
    }
    return sum;
}
