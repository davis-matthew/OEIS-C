/*
 * A138823  Divisors of 248 (the 3rd perfect number divided by 2), written in
 *          base 2.
 * https://oeis.org/A138823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, less
 * Formula:   a(n) = the nth divisor of 248 written in base 2 as a decimal
 *            integer (i.e. the binary representation of the divisor
 *            interpreted as a number in base 10)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   _bin_as_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_bin_as_decimal.h"

/* List of all divisors of 248 in increasing order */
static const seq_t _divisors[8] = {1, 2, 4, 8, 31, 62, 124, 248};

seq_t a(idx_t n)
{
    if (n == 0 || n > 8) return 0;       /* undefined for n <= 0 or beyond 8 */
    return _bin_as_decimal(_divisors[n - 1]);
}
