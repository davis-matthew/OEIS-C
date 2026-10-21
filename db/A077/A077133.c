/*
 * A077133  a(n) is the difference between the sum of the first n even-indexed
 *          primes and the sum of the first n odd-indexed primes.
 * https://oeis.org/A077133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-1} (prime(2*i+2) - prime(2*i+1)) (OEIS
 *            A077133)
 * Range:     u8 n <= 50, u16 n <= 6128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Static storage for primes: enough for n up to ~10000 (20000 primes) */
#define PRIME_CAP 30000
static uint32_t prime_list[PRIME_CAP];
static size_t prime_count = 0;
static uint32_t prime_candidate = 2;

/* Generate the next prime and store it in prime_list */
static void generate_next_prime(void)
{
    if (prime_count == 0) {
        prime_list[prime_count++] = 2;
        prime_candidate = 3;
        return;
    }

    if (prime_candidate < 3)
        prime_candidate = 3;
    if ((prime_candidate & 1) == 0)   /* skip even numbers */
        prime_candidate++;

    for (;;) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_count; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > prime_candidate)
                break;
            if (prime_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count < PRIME_CAP) {
                prime_list[prime_count++] = prime_candidate;
            }
            prime_candidate += 2;
            break;
        }
        prime_candidate += 2;
    }
}

/* Ensure that we have at least 'need' primes in prime_list */
static void ensure_primes(size_t need)
{
    while (prime_count < need)
        generate_next_prime();
}

/* Cached a(n) values up to the current maximum n computed */
static seq_t *a_cache = NULL;          /* array of seq_t, index 1..max_n, a_cache[0] unused */
static size_t a_cache_size = 0;        /* allocated size (in elements) */
static size_t max_n_computed = 0;      /* maximum n for which a_cache is computed */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n<1 but return 0 */

    /* Allocate or grow a_cache if needed */
    if ((size_t)n + 1 > a_cache_size) {
        size_t new_size = ((size_t)n + 1) * 2;  /* grow for future calls */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        a_cache = tmp;
        a_cache_size = new_size;
        /* Initialize new slots to 0 (not strictly necessary) */
        for (size_t i = max_n_computed + 1; i < a_cache_size; ++i)
            a_cache[i] = (seq_t)0;
    }

    /* Compute a(n) for n > max_n_computed */
    if ((size_t)n > max_n_computed) {
        /* Need primes up to index 2*n */
        size_t required_prime_index = 2 * (size_t)n;  /* 1-based indices */
        ensure_primes(required_prime_index);          /* ensures prime_count >= required_prime_index */

        /* Compute a values incrementally */
        for (size_t i = max_n_computed + 1; i <= (size_t)n; ++i) {
            /* Zero-based indices for primes array:
               odd-index prime = prime_list[2*i-2]
               even-index prime = prime_list[2*i-1]            */
            seq_t odd  = (seq_t)prime_list[2 * i - 2];
            seq_t even = (seq_t)prime_list[2 * i - 1];
            seq_t diff = even - odd;
            a_cache[i] = a_cache[i - 1] + diff;
        }
        max_n_computed = (size_t)n;
    }

    return a_cache[(size_t)n];
}
