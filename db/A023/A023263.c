/*
 * A023263  Primes that remain prime through 2 iterations of function f(x) =
 *          8x + 7.
 * https://oeis.org/A023263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023263: primes p such that 8p+7 and 64p+63 are also primes.
   The function in this file builds a sieve up to a fixed upper bound,
   checks each prime for the two additional primality conditions,
   and caches the resulting terms.  Subsequent calls return the cached
   value.  The implementation works for the generic C++ arbitrary‑precision
   build as well as for C and the standard unsigned integer builds. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

/* Upper bound for the sieve – large enough to generate ~10 000 terms. */
#define SIEVE_LIMIT 80000000UL

/* Bit‑set helpers for odd numbers only. */
#define ODD_INDEX(n)  (((n) - 3) >> 1)
#define BIT_OFF(idx)  ((idx) & 7U)
#define BYTE_OFF(idx) ((idx) >> 3)

/* Mark a number as composite. */
static void set_composite(uint8_t *sieve, uint32_t n)
{
    uint32_t idx = ODD_INDEX(n);
    sieve[BYTE_OFF(idx)] |= (1U << BIT_OFF(idx));
}

/* Test if a number is composite. */
static bool is_composite(const uint8_t *sieve, uint32_t n)
{
    if ((n & 1U) == 0U) return n != 2U;
    if (n < 3U) return false;
    uint32_t idx = ODD_INDEX(n);
    return (sieve[BYTE_OFF(idx)] >> BIT_OFF(idx)) & 1U;
}

/* Global cached data structures. */
static uint8_t  *sieve      = NULL;
static uint32_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;
static seq_t   *term_list  = NULL;
static size_t   term_cnt   = 0;
static size_t   term_cap   = 0;
static size_t   prime_pos  = 0;  /* next prime to examine */
static bool     initialized = false;

/* Allocate or enlarges an array. */
#define GROW_ARRAY(ptr, cap, need, type)                                   \
    do {                                                                    \
        if ((cap) < (need)) {                                               \
            size_t new_cap = (need);                                        \
            type *tmp = (type*)realloc((void*)(ptr), new_cap * sizeof(type)); \
            if (!tmp) exit(EXIT_FAILURE);                                  \
            (ptr) = tmp;                                                    \
            (cap) = new_cap;                                                \
        }                                                                   \
    } while (0)

/* Initialise the sieve and prime list once. */
static void init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate bitset for odd numbers up to SIEVE_LIMIT. */
    size_t odds  = (SIEVE_LIMIT - 1) >> 1;
    size_t bytes = (odds + 7) >> 3;
    sieve = (uint8_t*)calloc(bytes, 1);
    if (!sieve) exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes on odd numbers. */
    uint32_t i, j;
    for (i = 3; (uint64_t)i * i <= SIEVE_LIMIT; i += 2)
        if (!is_composite(sieve, i))
            for (j = i * i; j <= SIEVE_LIMIT; j += i << 1)
                set_composite(sieve, j);

    /* Rough estimate of number of primes for allocation size. */
    prime_cap = 6000000;   /* over‑allocate to avoid realloc during build */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    /* Build the prime list. */
    prime_cnt = 0;
    if (SIEVE_LIMIT >= 2) prime_list[prime_cnt++] = 2;
    for (i = 3; i <= SIEVE_LIMIT; i += 2)
        if (!is_composite(sieve, i)) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                GROW_ARRAY(prime_list, prime_cap, prime_cap, uint32_t);
            }
            prime_list[prime_cnt++] = i;
        }

    /* Initialise term cache. */
    term_cap  = 20000;
    term_list = (seq_t*)malloc(term_cap * sizeof(seq_t));
    if (!term_list) exit(EXIT_FAILURE);
}

/* Ensure that the cache has at least 'need' terms. */
static void ensure_terms(size_t need)
{
    init_sieve();

    while (term_cnt < need) {
        if (prime_pos >= prime_cnt) {
            /* Sieve too small for required terms – abort. */
            exit(EXIT_FAILURE);
        }
        uint32_t p = prime_list[prime_pos++];
        uint64_t x = 8ULL * (uint64_t)p + 7ULL;
        uint64_t y = 64ULL * (uint64_t)p + 63ULL;

        if (is_prime_small((seq_t)x) && is_prime_small((seq_t)y)) {
            if (term_cnt == term_cap) {
                term_cap <<= 1;
                GROW_ARRAY(term_list, term_cap, term_cap, seq_t);
            }
            term_list[term_cnt++] = (seq_t)p;
        }
    }
}

/* Public function that returns the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset – safe default */
    ensure_terms((size_t)n);
    return term_list[(size_t)(n - 1)];
}
