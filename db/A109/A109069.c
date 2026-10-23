/*
 * A109069  Numbers n such that the string n2323 is prime.
 * https://oeis.org/A109069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 8529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal concatenation of n and 2323
   yields a prime number, i.e. (n*10000 + 2323) is prime.  The function
   below returns the n-th such number, with a(1) = 1.  It is implemented
   by a simple incremental search using a 64‑bit primality test. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* The successive numbers to test. */
    idx_t candidate = 1;
    idx_t found = 0;

    while (true)
    {
        /* Form the number n*10000 + 2323.  Use 128‑bit arithmetic to
           avoid overflow for very large candidates.  For the range of
           indices we care about (< 1e6) this fits in 64 bits. */
        __uint128_t value128 = (__uint128_t)candidate * 10000ull + 2323ull;
        uint64_t value = (uint64_t)value128;   /* safe because value < 2^64 */

        if (is_prime_uint64(value))
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
