/*
 * A035279  One tenth of deca-factorial numbers.
 * https://oeis.org/A035279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^(n-1) * n! (for n >= 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n! and 10^(n-1), then multiply. */
    seq_t fact = 1;
    seq_t pow10 = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;            /* fact = i!   */
        pow10 *= (seq_t)10;          /* 10^(i-1)   */
    }
    return fact * pow10;             /* 10^(n-1) * n! */
}
