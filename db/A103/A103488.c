/*
 * A103488  a(n) = 2^(n^2-1).
 * https://oeis.org/A103488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n^2-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, below 1 is undefined */
    seq_t exp   = (seq_t)n * (seq_t)n - 1; /* exponent n^2 - 1 */
    seq_t res   = 1;                 /* result starts from 1 */
    seq_t base  = 2;                 /* base 2 */

    /* fast binary exponentiation */
    while (exp > 0) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}
