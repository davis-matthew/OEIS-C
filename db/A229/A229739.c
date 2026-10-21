/*
 * A229739  a(n) = q^2*(q^2+2*q-1)/2, where q = n-th prime power A000961(n).
 * https://oeis.org/A229739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q^2 * (q^2 + 2*q - 1) / 2, where q = nth prime power
 *            A000961(n)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* Cache of the first prime powers, stored as uint64_t */
static uint64_t *prime_powers = NULL;
static size_t    pp_count   = 0;
static size_t    pp_cap     = 0;
static uint64_t  next_candidate = 1;

/* Return true iff m is a prime power (including m == 1).  */
static bool is_prime_power(uint64_t m)
{
    if (m == 1)
        return true;
    return count_distinct_primes(m) == 1;
}

/* Extend the cache until it contains at least 'target' prime powers. */
static void extend_prime_powers(size_t target)
{
    if (pp_count >= target)
        return;

    while (pp_count < target) {
        if (is_prime_power(next_candidate)) {
            if (pp_count == pp_cap) {
                pp_cap = pp_cap ? pp_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_powers, pp_cap * sizeof(uint64_t));
                prime_powers = tmp;
            }
            prime_powers[pp_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* defined for n ≥ 1; return 0 for safety */
        return 0;

    extend_prime_powers((size_t)n);

    seq_t q = (seq_t)prime_powers[n - 1];
    seq_t qsq = q * q;
    seq_t tmp = qsq + (q << 1) - (seq_t)1;   /* qsq + 2*q - 1 */
    seq_t res = qsq * tmp / (seq_t)2;
    return res;
}
