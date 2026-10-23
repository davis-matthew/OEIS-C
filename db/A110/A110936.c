/*
 * A110936  a(n) = denominator(Bernoulli(prime(n) - 1))/prime(n).
 * https://oeis.org/A110936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator(Bernoulli(prime(n) - 1))/prime(n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 41, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based) as a seq_t and cast to uint64_t.  */
    uint64_t p = (uint64_t)get_prime(n);
    /* m = prime(n) - 1 : small enough to use 64‑bit arithmetic.  */
    uint64_t m = p - 1;
    seq_t denom = 1;

    /* Enumerate divisors of m by trial up to sqrt(m).  */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* First divisor d.  */
            uint64_t cand = d + 1;          /* prime candidate */
            if (is_prime(cand))
                denom = denom * (seq_t)cand;
            /* Complementary divisor m/d, if distinct.  */
            uint64_t e = m / d;
            if (e != d) {
                cand = e + 1;
                if (is_prime(cand))
                    denom = denom * (seq_t)cand;
            }
        }
    }

    /* Divide by the prime itself – it must divide the denominator.  */
    return denom / (seq_t)p;
}
