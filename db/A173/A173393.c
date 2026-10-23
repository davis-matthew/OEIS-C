/*
 * A173393  a(n) = 10*n + 12^n.
 * https://oeis.org/A173393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n + 12^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1 via direct exponentiation in seq_t */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n<1, return 0 for safety */

    seq_t pow12 = (seq_t)1; /* 12^0 */
    for (idx_t i = 0; i < n; ++i)
        pow12 *= (seq_t)12; /* multiply by 12 n times */

    return (seq_t)10 * (seq_t)n + pow12;
}
