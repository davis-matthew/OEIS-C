/*
 * A074302  Sum of squares of digits of n is equal to the largest prime factor
 *          of n.
 * https://oeis.org/A074302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074302:
   a(n) is the nth positive integer m (with a(0)=1) such that
     sum of squares of the decimal digits of m equals the largest
     prime factor of m.  The sequence is computed by a lazy linear
   search; no closed formula exists. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_square_sum.h"  /* uint64_t digit_square_sum(uint64_t) */

/* ------------ prime sieve for 0 .. 1'000'000 ----------------- */
static const uint64_t PRIME_LIMIT = 1000000ULL;
static uint32_t *prime_list   = NULL;   /* primes < PRIME_LIMIT+1 */
static size_t   prime_count   = 0;

/* Initialise prime_list via a simple sieve of Eratosthenes. */
static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already initialised */

    bool *is_composite = (bool *)calloc((size_t)PRIME_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    is_composite[0] = is_composite[1] = true;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_composite[(size_t)i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[(size_t)j] = true;
        }
    }

    /* Count primes */
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[(size_t)i]) ++prime_count;

    /* Allocate array */
    prime_list = (uint32_t *)calloc(prime_count, sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    /* Fill array */
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[(size_t)i]) prime_list[idx++] = (uint32_t)i;

    free(is_composite);
}

/* ------------ largest prime factor of a uint64_t ---------------- */
static uint64_t largest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;   /* lpf(0)=0, lpf(1)=1 */
    uint64_t temp = n;
    uint64_t max_prime = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (temp % p == 0) {
            max_prime = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) max_prime = temp;   /* remaining factor is prime */
    return max_prime;
}

/* ------------ sequence cache ---------------------------------- */
static seq_t *term_cache   = NULL;   /* array of seq_t values */
static size_t term_count   = 0;      /* number of terms stored */
static size_t term_cap     = 0;      /* current capacity */

/* Resize cache to accommodate at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (term_cap >= need) return;
    size_t new_cap = term_cap ? term_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    term_cache = tmp;
    term_cap = new_cap;
}

/* ------------ lazy generation of the sequence ----------------- */
static uint64_t next_candidate = 1;   /* next integer to test */

static void ensure_terms(idx_t n)
{
    init_primes();                       /* ensure prime list ready */

    while (term_count <= (size_t)n) {
        uint64_t m = next_candidate++;
        uint64_t lpf = largest_prime_factor(m);
        uint64_t dss = digit_square_sum(m);     /* from utils */
        if (lpf == dss) {
            ensure_cache_capacity(term_count + 1);
            term_cache[term_count++] = (seq_t)m;
        }
    }
}

/* ------------ public entry point ----------------------------- */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;          /* safety – undefined for negative n */
    ensure_terms(n);
    return term_cache[(size_t)n];
}
