/*
 * A049584  Primes p such that x^52 = 2 has a solution mod p.
 * https://oeis.org/A049584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that the congruence x^52 ≡ 2 (mod p) has a
 *            solution
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_small, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"
#include "../utils/powmod64.h"

/* Euclidean GCD for 64‑bit unsigned integers. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*
 * Returns the n‑th element of OEIS sequence A049584.
 * The function is memoised and works for arbitrary‑precision seq_t.
 */
seq_t a(idx_t n)
{
    /* Static storage for memoised qualifying primes. */
    static seq_t *good_primes = NULL;
    static size_t good_cnt   = 0; /* number of primes stored */
    static size_t good_cap   = 0; /* capacity of good_primes array */
    static uint64_t last_checked = 3; /* next integer to test for primality */

    /* Initialise the cache with the first term, 2. */
    if (good_cnt == 0) {
        /* Append the prime 2. */
        append_candidate(&good_primes, &good_cnt, &good_cap, (seq_t)2);
        /* Start checking odd integers from 3. */
        last_checked = 3;
    }

    /* Generate primes until we have at least n qualifying ones. */
    while (good_cnt < (size_t)n) {
        uint64_t p = last_checked;
        ++last_checked;               /* advance for next iteration */

        /* Skip even numbers except 2. */
        if ((p & 1ULL) == 0)
            continue;

        /* Test primality. */
        if (!is_prime_small((seq_t)p))
            continue;

        /* Special‑case p = 2 is already handled. */
        if (p == 2)
            continue;

        /* For odd primes: check 2^((p-1)/gcd(52,p-1)) ≡ 1 (mod p). */
        uint64_t g   = gcd_u64(52ULL, p - 1ULL);
        uint64_t exp = (p - 1ULL) / g;
        uint64_t val = powmod64(2ULL, exp, p);
        if (val == 1ULL)
            append_candidate(&good_primes, &good_cnt, &good_cap, (seq_t)p);
    }

    /* Return the requested term.  The array is 0‑based while OEIS indices start at 1. */
    return good_primes[n - 1];
}
