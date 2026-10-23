/*
 * A024628  Odd numbers k such that neither k nor k + 2 is a power of a prime.
 * https://oeis.org/A024628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer k such that neither k nor k+2 is a
 *            power of a prime (i.e., not equal to p^e with p prime, e>=1)
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Warning: For each candidate, we test primality factors via
   _smallest_prime_factor.  The function returns 0 if it cannot find a factor,
   but for positive integers up to 64‑bit this should never happen. */

#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

static bool is_prime_power(uint64_t n)
{
    if (n < 2) return false;               /* 1 is not a prime power */
    uint64_t spf = _smallest_prime_factor(n);
    if (spf == 0) return false;            /* safety fallback, should not occur */
    while (n % spf == 0) n /= spf;
    return n == 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 3;                          /* start with the first odd >1 */

    for (;; k += 2) {
        if (!is_prime_power(k) && !is_prime_power(k + 2)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
