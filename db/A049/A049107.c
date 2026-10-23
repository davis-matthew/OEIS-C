/*
 * A049107  a(n) = Euler phi function applied 5 times to n.
 * https://oeis.org/A049107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(phi(phi(phi(phi(n)))))
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;               /* cast index to the value type */
    for (int i = 0; i < 5; ++i) {
        x = phi((idx_t)x);            /* apply Euler's totient function */
    }
    return x;
}
