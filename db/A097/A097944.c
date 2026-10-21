/*
 * A097944  Number of digits in n-th prime.
 * https://oeis.org/A097944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of the n‑th prime, a(1) = 1,
 *            a(2) = 1,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Count its decimal digits.  digit_count returns a uint64_t,
       which easily fits into any seq_t. */
    return (seq_t)digit_count((uint64_t)p);
}
