/*
 * A107279  a(n) = 1 if n is an odd prime, a(n) = 2 if n is a nonzero even
 *          number, otherwise a(n) = 0.
 * https://oeis.org/A107279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is an odd prime, a(n) = 2 if n is a positive even
 *            number, a(n) = 0 otherwise
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;
    if ((n & 1) == 0)           /* even n > 0 */
        return 2;
    return is_prime(n) ? 1 : 0; /* odd n */
}
