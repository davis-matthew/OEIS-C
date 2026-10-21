/*
 * A227078  The Ramanujan-Nagell squares: A038198(n)^2.
 * https://oeis.org/A227078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) + 7 = 2^A060728(n) (equivalently a(n) = (2*x-1)^2 where x
 *            solves x*(x-1)/2 + 1 = 2^z)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is finite; after the 5 known solutions no terms are defined. */
    if (n >= 5)
        return 0;

    idx_t z = 0;
    idx_t found = 0;
    while (1) {
        /* Compute D = 2^(z+3) - 7 */
        seq_t D = pow2(z + 3);          /* 2^(z+3) */
        D -= 7;                         /* D = 2^(z+3) - 7 */

        /* Check if D is a perfect square */
        seq_t s = floor_sqrt(D);
        if (s * s == D) {
            if (found == n)
                return D;
            found++;
        }
        ++z;
    }
}
