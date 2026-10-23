/*
 * A101040  If n has one or two prime-factors then 1 else 0.
 * https://oeis.org/A101040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if Ω(n) <= 2 (n has one or two prime factors counted
 *            with multiplicity), else 0. * Ω(n) is the total number of prime
 *            factors of n, counted with multiplicity
 * Range:     u8 n <= 8109, u16 n <= 8109, u32 n <= 8109, u64 n <= 8109,
 *            u128 n <= 8109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Static list of primes used for factorisation.  The list is grown on demand. */
static uint64_t prime_list[110000];
static uint32_t prime_count = 0;
static uint64_t prime_last_n = 1;

/* Ensure that prime_list contains all primes up to 'limit'. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_last_n >= limit)
        return;

    for (uint64_t cand = prime_last_n + 1; cand <= limit; ++cand) {
        bool isprime = true;
        for (uint32_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime)
            prime_list[prime_count++] = cand;
    }
    prime_last_n = limit;
}

/* Counts the total number of prime factors of n (with multiplicity).  Stops counting
 * as soon as more than 2 prime factors are found, since we only need to know if the
 * count is 1 or 2. */
static uint32_t omega_count(uint64_t n)
{
    uint32_t count = 0;
    uint64_t remaining = n;

    /* We need primes up to sqrt(remaining).  While we factorize, the bound decreases,
       so we just generate primes up to the initial sqrt(n) once. */
    uint64_t sqrt_n = (uint64_t) ((double)remaining * 0.0 + 1.0);   /* dummy to avoid warning */
    sqrt_n = 1;
    while (sqrt_n * sqrt_n <= remaining) ++sqrt_n;
    sqrt_n -= 1;

    ensure_primes_up_to(sqrt_n);

    for (uint32_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= remaining; ++i) {
        uint64_t p = prime_list[i];
        while (remaining % p == 0) {
            ++count;
            remaining /= p;
            if (count > 2)
                return count;
        }
    }
    if (remaining > 1)
        ++count;
    return count;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    uint64_t nn = (uint64_t)n;
    uint32_t omega = omega_count(nn);
    return (seq_t)((omega == 1 || omega == 2) ? 1 : 0);
}
