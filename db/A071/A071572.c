/*
 * A071572  Least k such that nextprime(k*prime(n)) - k*prime(n) is nonprime.
 * https://oeis.org/A071572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A071572
 *
 *  a(n) = least k >= 1 such that next_prime(k * p_n) - k*p_n
 *  is not prime, where p_n is the n-th prime.
 *
 *  Implementation uses the provided get_prime() and next_prime()
 *  utilities and a simple trial‑division primality test for the
 *  small differences involved.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

/* trial division primality test for 64‑bit values;
   fast enough because differences are tiny */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t p = get_prime(n);       /* n-th prime */
    for (uint64_t k = 1; ; ++k) {
        uint64_t prod = p * k;               /* k * p */
        uint64_t nxt  = next_prime(prod);    /* next prime > prod */
        uint64_t diff = nxt - prod;          /* difference */

        if (!is_prime_uint64(diff))   /* diff composite or not prime */
            return (seq_t)k;          /* k is the least such index */
    }
}
