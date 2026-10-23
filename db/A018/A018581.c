/*
 * A018581  Divisors of 665.
 * https://oeis.org/A018581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 665 in increasing order
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static const seq_t _divisors[8] = {1,5,7,19,35,95,133,665};
seq_t a(idx_t n)
{
    if (n < 1 || n > 8) return 0;
    return _divisors[n-1];
}
