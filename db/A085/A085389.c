/*
 * A085389  a(1) = 1; for n >= 2, a(n) = (n*(n+1)^(n-1))/(n+1).
 * https://oeis.org/A085389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)^(n-2) for n >= 2, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    seq_t base = (seq_t)n + 1;               /* (n+1) */
    seq_t exp  = (seq_t)(n - 2);             /* exponent (n-2) */
    seq_t pw   = big_pow(base, exp);         /* (n+1)^(n-2) */
    return (seq_t)n * pw;                    /* n * (n+1)^(n-2) */
}
