/*
 * A078788  a(n) is the smallest m > 2 such that (1 - n) mod m = 0, or 0 if no
 *          such number exists.
 * https://oeis.org/A078788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime factor of (n-1), or 0 if (n-1) has no
 *            * such factor (i.e., if n-1 is a power of 2). This implements
 *            the definition * of OEIS sequence A078788
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   smallest_odd_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_odd_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;     /* offset; behaviour for n < 2 is unspecified */

    uint64_t x = (uint64_t)(n - 1);
    uint64_t p = smallest_odd_prime_factor(x);
    return (seq_t)p;
}
