/*
 * A078285  Least nontrivial multiple of the n-th prime beginning with 1.
 * https://oeis.org/A078285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = ceil(10^d / prime(n)) * prime(n) where d =
 *            digit_len(prime(n))
 * Range:     u8 n <= 25, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses helper functions: get_prime, pow10, digit_len */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* 1‑based index: obtain the n-th prime */
    uint32_t small = get_prime(n);
    seq_t p = (seq_t)small;

    /* Number of decimal digits of the prime */
    int d = digit_len(p);

    /* Compute 10^d */
    seq_t pow10_d = pow10(d);

    /* Ceiling of pow10_d / p: ceil(x/y) = (x + y - 1) / y */
    seq_t mul = (pow10_d + p - 1) / p;

    /* Least multiple of p that starts with 1 */
    return mul * p;
}
