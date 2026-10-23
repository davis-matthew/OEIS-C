/*
 * A105661  a(n)=1 if n is a prime, 2 if n is an even semiprime, otherwise 0.
 * https://oeis.org/A105661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is prime, 2 if n is an even semiprime (n = 2*p, p
 *            prime), 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Small deterministic primality test for 64‑bit unsigned integers.
   Works for any n up to 2^64 – the algorithm is trial division up to
   sqrt(n), using i <= n / i to avoid overflow. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Convert the index to the integer type used by the helpers. */
    uint64_t v = (uint64_t)n;

    if (is_prime_uint64(v))
        return (seq_t)1;

    if ((v & 1ULL) == 0) {            /* even */
        uint64_t half = v >> 1;
        if (is_prime_uint64(half))
            return (seq_t)2;
    }

    return (seq_t)0;
}
