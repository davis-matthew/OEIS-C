/*
 * A205707  Prime(A205705(n)), the n-th number s(k) such that 8 divides
 *          s(k)-s(j) for some j<k, where s(j)=prime(j).
 * https://oeis.org/A205707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(A205705(n)), * where A205705(n) is the n-th index k
 *            of a prime p_k * such that there exists j<k with 8 | (p_k -
 *            p_j). * The pairs (j,k) are ordered by ascending k and, for each
 *            k, * by ascending difference (equivalently, by descending j)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static uint64_t *prime_arr = NULL;          /* 1‑based array of primes */
static size_t prime_cap = 0;                /* current capacity of prime_arr */
static idx_t primes_count = 0;              /* number of primes stored (>=1) */
static uint64_t next_candidate = 3;         /* next odd number to test for primality */

static seq_t *results = NULL;               /* cached a(n) values */
static size_t result_cap = 0;               /* capacity of results array */
static size_t result_count = 0;             /* number of results stored */

/* Ensure the prime array is initialized with the first prime 2. */
static void init_prime_array(void)
{
    if (primes_count == 0) {
        if (prime_cap < 16) {
            prime_cap = 16;
            prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
        }
        primes_count = 1;
        prime_arr[1] = 2;
        next_candidate = 3;
    }
}

/* Append a value to the results array, growing it if necessary. */
static void append_result(seq_t val)
{
    if (result_count == result_cap) {
        size_t new_cap = result_cap == 0 ? 64 : result_cap << 1;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        results = tmp;
        result_cap = new_cap;
    }
    results[result_count++] = val;
}

/* Generate the next prime number, store it, and return its value. */
static uint64_t generate_next_prime(void)
{
    init_prime_array();
    while (true) {
        uint64_t p = next_candidate;
        next_candidate += 2;              /* only test odd numbers */
        bool is_prime = true;
        for (size_t i = 1; i <= primes_count; ++i) {
            uint64_t q = prime_arr[i];
            if (q * q > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_count + 1 > prime_cap) {
                size_t new_cap = prime_cap == 0 ? 16 : prime_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
                prime_arr = tmp;
                prime_cap = new_cap;
            }
            ++primes_count;
            prime_arr[primes_count] = p;
            return p;
        }
    }
}

/* Ensure that a(n) has been computed up to index n. */
static void ensure_results_upto(idx_t n)
{
    while (result_count < n) {
        uint64_t new_prime = generate_next_prime();   /* primes_count is updated */
        idx_t k = primes_count;
        /* Scan earlier primes in descending order to get ascending differences. */
        for (idx_t j = k - 1; j > 0; --j) {
            if (((new_prime - prime_arr[j]) % 8) == 0) {
                append_result((seq_t)get_prime(k));
            }
        }
    }
}

/* Public function required by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe return */
    ensure_results_upto(n);
    return results[n - 1];
}
