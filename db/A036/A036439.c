/*
 * A036439  a(n) = a(n-1) + prime(n-1), with a(1)=2.
 * https://oeis.org/A036439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + prime(n-1), a(1) = 2
 * Range:     u8 n <= 14, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External utilities (not used directly) */
#define _USE_MATH_DEFINES
/* The only required function is a trial division using previously found primes. */

/* Helper to test primality using previously found primes */
static int is_prime_trial(uint64_t x, const uint64_t *prime_list, size_t prime_count)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Main function computing the requested term */
seq_t a(idx_t n)
{
    /* Static storage for computed sequence values and primes */
    static seq_t *seq_vals = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len = 0;          /* highest index computed (inclusive) */

    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0;
    static size_t prime_len = 0;        /* number of primes stored */

    /* Handle n = 0 gracefully (outside offset) */
    if (n == 0) return 0;

    /* Allocate initial storage for sequence values when first called */
    if (seq_vals == NULL) {
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_vals) return 0;          /* allocation failed, return 0 */
        seq_vals[0] = 0;                 /* unused, keep 1-based indexing */
        seq_vals[1] = 2;                 /* a(1) = 2 */
        seq_len = 1;
    }

    /* Grow the sequence array if needed */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        seq_vals = tmp;
        seq_cap  = new_cap;
    }

    /* Allocate initial storage for primes when first needed */
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) return seq_vals[n]; /* allocation failed, continue */
    }

    /* Ensure we have at least (n-1) primes (prime(1) = 2) */
    while (prime_len < (size_t)(n - 1)) {
        /* Find next prime after the current last one */
        uint64_t candidate = (prime_len == 0) ? 2 : prime_list[prime_len - 1] + 1;
        for (;;) {
            if (is_prime_trial(candidate, prime_list, prime_len)) break;
            ++candidate;
        }

        /* Grow prime array if necessary */
        if (prime_len + 1 > prime_cap) {
            size_t new_cap = prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!tmp) break; /* allocation failed, exit loop */
            prime_list = tmp;
            prime_cap  = new_cap;
        }
        prime_list[prime_len] = candidate;
        ++prime_len;
    }

    /* Compute new terms up to n if they don't exist yet */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        /* prime(i-1) is prime_list[i-2] because prime_list is 0-based */
        seq_t p = (seq_t)prime_list[i - 2];
        seq_vals[i] = seq_vals[i - 1] + p;
    }
    if (n > seq_len) seq_len = n;

    return seq_vals[n];
}
