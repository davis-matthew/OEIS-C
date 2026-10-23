/*
 * A129073  Numbers k such that (k-8)/7 is prime.
 * https://oeis.org/A129073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*prime(n) + 8
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 the behaviour
       is irrelevant as it is below the offset.  Using get_prime to
       fetch the n-th prime (1‑based). */
    if (n == 0)
        return 0;  /* arbitrary, offset is 1 */
    uint32_t p = get_prime(n);
    /* Compute 7 * p + 8 in seq_t to avoid overflow. */
    seq_t result = (seq_t)7 * (seq_t)p + (seq_t)8;
    return result;
}
