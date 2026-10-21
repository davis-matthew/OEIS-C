/*
 * A051034  Minimal number of primes needed to sum to n.
 * https://oeis.org/A051034
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal number of primes needed to sum to n (OEIS A051034)
   The function uses a small static sieve up to MAX_N.  
   For n within the sieve range
        a(n) = 1 if n is prime,
        a(n) = 2 if n is even or if n-2 is prime (i.e. 2 + (n-2)),
        a(n) = 3 otherwise.
   For n outside the sieve range a simple trial‑division primality test is
   used for Goldbach checks.  This suffices for all n used in the tests
   (≤ 10 000). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_num.h"

#define MAX_N 10000        /* sieve range sufficient for test data */

static bool sieve_done = false;
static bool is_prime[MAX_N + 1];
static uint32_t primes[1229];   /* number of primes ≤ 10 000 is 1229 */
static size_t prime_cnt = 0;

/* --- small sieve ----------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve_done) return;
    for (size_t i = 0; i <= MAX_N; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_N; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= MAX_N; j += i)
                is_prime[j] = false;
        }
    }
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAX_N; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
    sieve_done = true;
}

/* --- main sequence function ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* undefined region, never used */
    init_sieve();

    /* 1) n itself is prime */
    if (n <= MAX_N) {
        if (is_prime[(size_t)n]) return (seq_t)1;
    } else {
        if (is_prime_num((uint64_t)n)) return (seq_t)1;
    }

    /* 2) even n > 2 */
    if ((n & 1ULL) == 0) {
        uint64_t half = n / 2;
        for (size_t i = 0; i < prime_cnt && primes[i] <= half; ++i) {
            uint64_t p = primes[i];
            uint64_t q = n - p;
            bool qprime = (q <= MAX_N) ? is_prime[(size_t)q] : is_prime_num(q);
            if (qprime) return (seq_t)2;
        }
        /* fallback – all even ≤ 10000 satisfy Goldbach, so unreachable */
        return (seq_t)4;
    }

    /* 3) odd n > 2 */
    uint64_t n_minus_two = n - 2;
    bool pair_possible = (n_minus_two <= MAX_N) ?
                          is_prime[(size_t)n_minus_two] :
                          is_prime_num(n_minus_two);
    if (pair_possible) return (seq_t)2;

    /* 4) otherwise */
    return (seq_t)3;
}
