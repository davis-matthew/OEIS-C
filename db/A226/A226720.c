/*
 * A226720  Complement of A122437.
 * https://oeis.org/A226720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor( n * (1 + log_3(2)) )
 * Range:     u8 n <= 156, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* y = 1 + log_3(2) */
    static const long double y = 1.63092975357145743719L;

    /* compute floor(n*y) */
    long double v = (long double)n * y;
    unsigned long long k = (unsigned long long)floorl(v);

    return (seq_t)(k + 1);
}
