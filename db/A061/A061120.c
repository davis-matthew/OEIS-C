/*
 * A061120  Compute sum of divisors of the prime(n+1)-prime(n)-1 composite
 *          numbers between two consecutive primes; choose the smallest.
 * https://oeis.org/A061120
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{σ(c); p(n+1) > c > p(n)}, where σ = sum of divisors,
 *            and c is composite
 * Range:     u8 n <= 27, u16 n <= 2325, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   get_prime, is_prime_small, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2: a(2) is the first defined term. */
    if (n < 2) return 0;          /* undefined for n < 2, trivial value */

    /* Get consecutive primes p(n) and p(n+1). */
    idx_t p_n  = get_prime(n);
    idx_t p_n1 = get_prime(n + 1);

    seq_t min_sigma = 0;
    bool first = true;

    /* Scan the open interval (p_n, p_n1). */
    for (idx_t c = p_n + 1; c < p_n1; ++c) {
        /* Skip primes; only composites are considered. */
        if (is_prime_small((uint64_t)c))
            continue;

        seq_t sigma_c = sigma_of(c);

        if (first || sigma_c < min_sigma) {
            min_sigma = sigma_c;
            first = false;
        }
    }

    return min_sigma;
}
