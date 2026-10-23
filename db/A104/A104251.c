/*
 * A104251  Sum of nonprime digits of n-th prime.
 * https://oeis.org/A104251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of nonprime digits of the n-th prime (A104251)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   nonprime_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/nonprime_digit_sum.h"

/* Helper: determine if n is prime (trial division, fast enough for n ≤ ~105000) */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* Cache of primes discovered so far */
static uint64_t *prime_list   = NULL;
static idx_t     prime_count  = 0;
static idx_t     prime_cap    = 0;

/* Ensure that prime_list contains at least n primes */
static void ensure_n_primes(idx_t n)
{
    if (prime_count >= n) return;

    /* grow capacity if needed */
    if (prime_cap == 0) {
        prime_cap = 8;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    uint64_t candidate = (prime_count == 0) ? 2ULL : prime_list[prime_count - 1] + 1ULL;

    while (prime_count < n) {
        if (is_prime_uint64(candidate)) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset, return 0 */
    ensure_n_primes(n);
    uint64_t nth_prime = prime_list[n - 1];
    return nonprime_digit_sum(nth_prime);
}
