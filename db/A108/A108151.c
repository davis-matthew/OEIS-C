/*
 * A108151  a(n) = n^2 + 3*n + 1 if prime or 0 if composite.
 * https://oeis.org/A108151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n^2 + 3*n + 1 if the value is prime, otherwise 0
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset: n >= 1 is required */
    uint64_t val = (uint64_t)n * ((uint64_t)n + 3) + 1;
    return is_prime(val) ? (seq_t)val : 0;
}
