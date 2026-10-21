/*
 * A092447  Concatenate odd primes in decreasing order.
 * https://oeis.org/A092447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate of the first n odd primes in decreasing order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities provided by the build environment */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"

/* Count decimal digits of a 32‑bit unsigned integer. */
static inline size_t digit_count_uint32(uint32_t x)
{
    size_t d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0) return result;

    /*  get_prime(1) == 2, so for n odd primes we start from index n+1 (the nth odd prime) */
    for (idx_t k = n + 1; k >= 2; --k)
    {
        uint32_t p = get_prime(k);            /* kth prime (primes[1] == 2) */
        size_t d = digit_count_uint32(p);     /* number of decimal digits of p */
        result = result * pow10((idx_t)d) + (seq_t)p;
    }
    return result;
}
