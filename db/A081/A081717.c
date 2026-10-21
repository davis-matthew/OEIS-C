/*
 * A081717  Primes prime(j) such that prime(j) + j is composite.
 * https://oeis.org/A081717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 5878, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081717 : primes p_j such that p_j + j is composite. */
/*  The function computes the n‑th such prime, with memoisation so that calls
    in any order are efficient. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *primes = NULL;          /* list of all primes found so far */
static size_t primes_count = 0;          /* number of primes in the list    */
static size_t primes_cap = 0;            /* allocated capacity of the list  */
static uint64_t last_checked_prime = 0; /* the last prime examined        */

static seq_t *valid = NULL;             /* list of primes satisfying the property */
static size_t valid_count = 0;           /* count of such primes               */
static size_t valid_cap = 0;             /* allocated capacity                 */

/* Ensure the primes array can hold at least 'need' elements. */
static void ensure_primes_cap(size_t need)
{
    if (need > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Ensure the valid array can hold at least 'need' elements. */
static void ensure_valid_cap(size_t need)
{
    if (need > valid_cap) {
        size_t new_cap = valid_cap ? valid_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(valid, new_cap * sizeof(seq_t));
        valid = tmp;
        valid_cap = new_cap;
    }
}

/* Simple deterministic primality test for 64‑bit numbers, using trial division
   by the primes we have already generated (up to sqrt(n)). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate primes sequentially until we have at least 'target' primes that
   satisfy the property. The function builds only what is needed. */
static void generate_required_primes(idx_t target)
{
    /* Initialise the primes list with 2 and 3 if empty. */
    if (primes_count == 0) {
        ensure_primes_cap(2);
        primes[0] = 2;
        primes[1] = 3;
        primes_count = 2;
        last_checked_prime = 3;
    }

    /* Candidate for next prime; start from the number after the last known prime. */
    uint64_t candidate = last_checked_prime + 1;
    if (candidate % 2 == 0) ++candidate;  /* skip even numbers except 2 */

    while (valid_count < (size_t)target) {
        /* Test primality of candidate. */
        if (is_prime_uint64(candidate)) {
            /* Record the new prime. */
            ensure_primes_cap(primes_count + 1);
            primes[primes_count++] = candidate;
            last_checked_prime = candidate;

            size_t j = primes_count;  /* 1‑based index of this prime */

            /* Check the property p_j + j composite. */
            uint64_t test_num = candidate + (uint64_t)j;
            if (!is_prime_uint64(test_num)) {
                ensure_valid_cap(valid_count + 1);
                valid[valid_count++] = (seq_t)candidate;
            }
        }
        candidate += 2;  /* only odd candidates needed beyond 2 */
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour below offset irrelevant */
    generate_required_primes(n);
    return valid[n - 1];
}
