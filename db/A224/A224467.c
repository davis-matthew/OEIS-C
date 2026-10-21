/*
 * A224467  Numbers n such that 27*n+1 is prime.
 * https://oeis.org/A224467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer n for which 27*n+1 is prime
 * Range:     u8 n <= 2, u16 n <= 527, u32 n <= 96, u64 n <= 5736,
 *            u128 n <= 5736, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* the OEIS offset is 1; index 0 is undefined */
        return 0;

    idx_t  found  = 0;   /* how many suitable numbers found so far   */
    seq_t  candidate = 0;/* current integer candidate for n         */

    while (found < n) {
        ++candidate;                              /* next integer to test     */
        seq_t v = (seq_t)27 * candidate + 1;      /* compute 27*n+1           */
        if (is_prime_big(v))
            ++found;                              /* counted as a valid term   */
    }

    return candidate;                             /* a(n) */
}
