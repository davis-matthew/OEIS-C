/*
 * A018283  Divisors of 100.
 * https://oeis.org/A018283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 2^((n-1) mod 3) * 5^floor((n-1)/3)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (n == 0) – unused. */
    if (n == 0u)
        return (seq_t)0;

    idx_t k = n - 1u;          /* n-1  */
    idx_t exp2 = k % 3u;       /* (n-1) mod 3  */
    idx_t exp5 = k / 3u;       /* floor((n-1)/3)  */

    seq_t val = pow2(exp2);        /* 2^exp2 */
    val *= power5(exp5);           /* * 5^exp5 */

    return val;
}
