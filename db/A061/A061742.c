/*
 * A061742  a(n) is the square of the product of first n primes.
 * https://oeis.org/A061742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ( ∏_{k=1..n} p_k )^2 where p_k is the k-th prime * (a(0)
 *            = 1). The implementation uses get_nth_prime(idx_t n) * provided
 *            by the utilities. It runs in O(n) multiplications and * works
 *            for any build (uint8_t … unsigned __int128 …
 *            arbitrary‑precision)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 92 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n‑th prime (n ≥ 1).  The primes are generated on demand
 * using a simple incremental sieve (trial division by known primes).
 * This provides enough primes for n up to at least 10 000. */
static uint32_t get_nth_prime(idx_t n)
{
    /* Number of primes discovered so far */
    static size_t primes_cnt = 0;
    /* Highest candidate examined so far */
    static uint32_t current = 2;
    /* Array of discovered primes */
    static uint32_t *primes = NULL;
    static size_t primes_cap = 0;

    /* Initialise storage if this is the first call */
    if (primes_cap == 0) {
        primes_cap = 64;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    }

    /* Grow the array if necessary */
    if (primes_cnt >= primes_cap) {
        primes_cap <<= 1;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }

    /* Fill in primes until we have at least n of them */
    while (primes_cnt < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > current)
                break;
            if (current % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[primes_cnt++] = current;

        /* Advance to next candidate (skip even numbers after 2) */
        current = (current == 2) ? 3 : current + 2;
    }

    return primes[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    seq_t primorial = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        primorial *= get_nth_prime(i);
    }
    return primorial * primorial;  /* square of the primorial */
}
