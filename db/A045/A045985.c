/*
 * A045985  a(n) = least k such that sum of first k primes is n times a prime.
 * https://oeis.org/A045985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 17, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: For a given n return the smallest k such that the sum
   of the first k primes equals n times a prime.  The routine generates
   primes and their prefix sums on demand and caches computed results. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* Global state for prime generation and sum storage */
static uint64_t *primes = NULL;      /* list of primes */
static size_t   prime_count = 0;     /* number of stored primes */
static size_t   prime_cap = 0;       /* capacity of primes array */

static seq_t   *sums = NULL;        /* prefix sums of primes */
static size_t   sum_count = 0;      /* number of stored sums */
static size_t   sum_cap = 0;        /* capacity of sums array */

static seq_t   current_sum = 0;     /* running sum of primes */

static uint64_t next_candidate = 2; /* next integer to test for primality */

/* Ensure capacity of primes array, reallocating if required */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    primes = tmp;
    prime_cap = new_cap;
}

/* Ensure capacity of sums array, reallocating if required */
static void ensure_sum_cap(size_t need)
{
    if (sum_cap >= need) return;
    size_t new_cap = sum_cap ? sum_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    sums = tmp;
    sum_cap = new_cap;
}

/* Append a new prime to the global state */
static void add_prime(uint64_t p)
{
    ensure_prime_cap(prime_count + 1);
    primes[prime_count++] = p;

    ensure_sum_cap(sum_count + 1);
    current_sum = current_sum + (seq_t)p;
    sums[sum_count++] = current_sum;
}

/* Test whether x (<= 2^64-1) is prime using trial division by stored primes */
static bool is_candidate_prime(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        /* stop when p*p > x to avoid overflow */
        if (p > x / p) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Generate the next prime, storing it and updating running sum */
static void generate_next_prime(void)
{
    /* Handle 2 explicitly then test odd numbers only */
    if (next_candidate == 2) {
        add_prime(2);
        next_candidate = 3;
        return;
    }

    for (;;) {
        if (is_candidate_prime(next_candidate)) {
            add_prime(next_candidate);
            next_candidate += 2;
            break;
        }
        next_candidate += 2;
    }
}

/* Ensure that sums[0 .. k-1] exist (i.e., first k primes generated) */
static void ensure_sums_up_to(idx_t k)
{
    while (sum_count < (size_t)k) {
        generate_next_prime();
    }
}

/* Cache for computed a(n) values, indexed by n-1 */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Main function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined at offset 0, return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Expand cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        /* Initialise new slots to 0 (unknown state) */
        if (new_cap > cache_cap)
            memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached result if available */
    if (cache[n - 1] != 0) {
        return cache[n - 1];
    }

    seq_t n_seq = (seq_t)n;
    idx_t k = 1;

    for (;;) {
        /* Make sure we have the sum for this k */
        ensure_sums_up_to(k);
        seq_t sum_k = sums[k - 1];

        /* Check if sum_k is a multiple of n */
        if ((sum_k % n_seq) == 0) {
            seq_t q = sum_k / n_seq;
            if (is_prime_big(q)) {
                cache[n - 1] = k;
                return k;
            }
        }
        ++k;
    }
}
