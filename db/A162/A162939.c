/*
 * A162939  A 1-based alternate sum over the numbers from 0 to prime(n).
 * https://oeis.org/A162939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1; otherwise a(n) = prime(n) + (prime(n)+1)/2
 *            where prime(n) is the n-th prime number. The implementation
 *            builds primes on demand using trial division. It works for any n
 *            >= 1, is independent of call order, and stays well below the
 *            time limit for n up to about 10,000
 * Range:     u8 n <= 39, u16 n <= 4551, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* For standard integer types */
#include <stddef.h>   /* For size_t */

static uint64_t primes[20000];   /* Enough for n up to ~20000 */
static size_t prime_cnt = 0;     /* Number of primes stored */
static uint64_t next_candidate = 3; /* Candidate to test for primality */

/* Small helper: check if x is divisible by any prime <= sqrt(x) */
static int is_prime_candidate(uint64_t x)
{
    if (x % 2 == 0) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure that primes[0..n-1] contain the first n primes */
static void ensure_primes(idx_t n)
{
    if (prime_cnt == 0) {
        primes[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;
    }
    while (prime_cnt < n) {
        /* test next_candidate until prime found */
        while (!is_prime_candidate(next_candidate))
            next_candidate += 2;   /* skip even numbers */
        primes[prime_cnt++] = next_candidate;
        next_candidate += 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but safe for out-of-range */
    if (n == 1) {
        /* a(1) = 1 */
        return (seq_t)1;
    }

    /* Ensure we have the nth prime */
    ensure_primes(n);

    /* Retrieve the n-th prime (1‑based indexing) */
    uint64_t p = primes[n - 1];
    /* Compute a(n) = p + (p+1)/2 */
    seq_t part = (seq_t)(p + 1) / 2;
    return (seq_t)p + part;
}
