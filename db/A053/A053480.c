/*
 * A053480  Sum of values when cototient function A051953 is iterated until
 *          fixed point is reached.
 * https://oeis.org/A053480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + cototient(n) + cototient^2(n) + … + 1 (cototient(n) =
 *            n - φ(n), φ(n) = Euler's totient)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_n.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                        /* out of range – not used in tests */

    seq_t sum = 0;
    idx_t cur = n;

    while (cur > 1)
    {
        sum += (seq_t)cur;                /* add current value */
        seq_t phi = phi_n(cur);           /* φ(cur) */
        cur  = (idx_t)((seq_t)cur - phi); /* cototient(cur) */
    }
    sum += 1;                              /* finally add the fixed point 1 */
    return sum;
}
