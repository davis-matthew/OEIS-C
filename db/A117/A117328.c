/*
 * A117328  Primes p such that p +- 4 are semiprimes.
 * https://oeis.org/A117328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 557, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A117328  
 *  a(n) = the n‑th prime p such that both p-4 and p+4 are semiprimes.
 *  The code tests successive candidates, using the provided helper
 *  _smallest_prime_factor() to determine primality and to factor the
 *  neighboring numbers.  It returns the prime as type seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

/* Test whether a 64‑bit number is prime. */
static inline bool is_prime_local(uint64_t x)
{
    return x >= 2 && _smallest_prime_factor(x) == x;
}

/* Test whether a 64‑bit number is a semiprime (product of two primes,
 * possibly equal).  The smallest semiprime is 4 = 2×2. */
static inline bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;
    uint64_t a = _smallest_prime_factor(x);
    if (a == x) return false;          /* prime */
    uint64_t b = x / a;
    return is_prime_local(b);
}

/* Return the n-th prime p such that p-4 and p+4 are semiprimes.  The
 * index is 1‑based (a(1) is the first such prime).  For n==0 a 0
 * placeholder is returned; the function is safe for any value of n
 * within the caller's domain. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, return 0 as safety */

    uint64_t count = 0;
    uint64_t candidate = 5;  /* first odd prime ≥ 5 */

    for (;;) {
        if (is_prime_local(candidate)) {
            if (is_semiprime(candidate - 4) && is_semiprime(candidate + 4)) {
                ++count;
                if (count == n) return (seq_t)candidate;
            }
        }
        candidate += 2;  /* skip even numbers */
    }
}
