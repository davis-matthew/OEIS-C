/*
 * A080951  Sequence associated with recurrence a(n) = 2*a(n-1) +
 *          k*(k+2)*a(n-2).
 * https://oeis.org/A080951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(n+4,4)*2^(n-1))
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 19, u64 n <= 47, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;      /* a(0) = 1 */

    /* binomial(n+4,4) = (n+1)(n+2)(n+3)(n+4) / 24 */
    seq_t bin = 1;
    for (uint8_t i = 1; i <= 4; ++i)
        bin = bin * (seq_t)(n + i);
    bin = bin / 24;   /* 4! = 24 */

    /* 2^(n-1) by repeated doubling (avoids shift UB) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow2 = pow2 * 2;

    return bin * pow2;
}
