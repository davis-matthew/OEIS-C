/*
 * A111171  Semiprimes S such that 3*S - 1 is also a semiprime.
 * https://oeis.org/A111171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th semiprime S such that 3*S-1 is also a semiprime
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Prime sieve up to PRIMES_LIMIT.  Values bigger than this are not
   required for the sequence up to n ≈ 10000. */
#define PRIMES_LIMIT 5000000U

static bool primes_initialized = false;
static unsigned char *prime_flag = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    size_t limit = PRIMES_LIMIT;
    prime_flag = (unsigned char *)malloc(limit + 1);
    if (!prime_flag)
        return;   /* Allocation failure */

    memset(prime_flag, 0, limit + 1);
    prime_flag[0] = prime_flag[1] = 0;
    for (size_t i = 2; i <= limit; ++i)
        prime_flag[i] = 1;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_flag[j] = 0;
        }
    }

    /* Allocate list of primes */
    size_t est = limit / 10 + 1;   /* Over‑estimate; plenty enough */
    prime_list = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!prime_list) {
        free(prime_flag);
        prime_flag = NULL;
        return;   /* Allocation failure */
    }

    for (size_t i = 2; i <= limit; ++i) {
        if (prime_flag[i]) {
            if (prime_count == est) { /* double capacity if needed */
                est *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, est * sizeof(uint32_t));
                if (!tmp) {
                    free(prime_list);
                    free(prime_flag);
                    prime_flag = NULL;
                    return;
                }
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    }

    primes_initialized = true;
}

/* Test if x is a semiprime (product of exactly two primes, not necessarily distinct) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;

    uint64_t tmp = x;
    int factors = 0;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        while (tmp % p == 0) {
            ++factors;
            tmp /= p;
            if (factors > 2)
                return false;
        }
    }

    if (tmp > 1)
        ++factors;   /* remaining prime factor */

    return factors == 2;
}

/* Dynamic storage for previously found terms. */
static seq_t *seq_terms = NULL;
static idx_t seq_capacity = 0;
static idx_t seq_size = 0;
static uint64_t current_candidate = 4;   /* first integer to test */

/* Append a new term to the sequence array. */
static void append_term(seq_t val)
{
    if (seq_size == seq_capacity) {
        idx_t new_cap = seq_capacity == 0 ? 128 : seq_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
        if (!tmp)
            return;   /* Allocation failure, silently ignore (unlikely) */
        seq_terms = tmp;
        seq_capacity = new_cap;
    }
    seq_terms[seq_size++] = val;
}

/* Requested OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset handling */

    init_primes();

    /* Generate sequence entries until we have at least n terms. */
    while (seq_size < n) {
        if (is_semiprime_uint64(current_candidate) &&
            is_semiprime_uint64(3 * current_candidate - 1ULL))
        {
            append_term((seq_t)current_candidate);
        }
        ++current_candidate;
    }

    return seq_terms[n - 1];
}
