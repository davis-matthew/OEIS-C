/*
 * A028915  Take next group of 3 primes, beginning at 7, map final digits to
 *          base 4 by 1->0, 3->1, 7->3, 9->2; convert back to decimal.
 * https://oeis.org/A028915
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   map_last_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A028915:  take the n‑th triple of primes (starting with 7,11,13), map
 *  the last decimal digit of each prime to a base‑4 digit (1→0, 3→1,
 *  7→3, 9→2), interpret the three digits as a base‑4 number, then return
 *  that number in decimal.  The sequence starts at offset 2, so
 *  a(2) is the first listed term.  This implementation generates
 *  primes on demand, caches them between calls, and works for any n≥2. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/map_last_digit.h"

/* Prime cache shared between calls. */
static uint64_t   *prime_list   = NULL;   /* array of primes          */
static size_t     prime_count  = 0;       /* number of primes stored  */
static size_t     prime_cap    = 0;       /* allocated array capacity */
static uint64_t   next_candidate = 2; /* next number to test      */

/* Ensure the prime cache has at least 'needed' slots. */
static void ensure_capacity_primes(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap : 8;
    while (new_cap < needed) new_cap <<= 1U;
    uint64_t *new_arr = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
    if (!new_arr) abort();          /* no memory */
    prime_list = new_arr;
    prime_cap  = new_cap;
}

/* Test whether a candidate number is prime using the cached primes. */
static inline int is_prime_candidate(uint64_t num)
{
    if (num < 2U) return 0;
    if ((num & 1U) == 0U) return (num == 2U);
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > num) break;
        if (num % p == 0U) return 0;
    }
    return 1;
}

/* Generate primes until we have at least 'needed_count' primes. */
static void generate_primes_upto(size_t needed_count)
{
    if (prime_count >= needed_count) return;
    while (prime_count < needed_count) {
        /* Find next prime. */
        while (!is_prime_candidate(next_candidate)) ++next_candidate;
        /* Store it. */
        ensure_capacity_primes(prime_count + 1);
        prime_list[prime_count++] = next_candidate;
        ++next_candidate;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 2U) return 0;          /* offset not defined below 2 */

    /* We need at least 3*n primes (the triple indices start at 0). */
    generate_primes_upto(3 * n);

    /* Index of first prime in the n-th triple (0‑based). */
    size_t idx = 3 * n - 3;
    uint64_t p1 = prime_list[idx];
    uint64_t p2 = prime_list[idx + 1];
    uint64_t p3 = prime_list[idx + 2];

    unsigned int d1 = map_last_digit(p1);
    unsigned int d2 = map_last_digit(p2);
    unsigned int d3 = map_last_digit(p3);

    /* Compute base‑4 number and convert to decimal (stored in seq_t). */
    seq_t value = ((seq_t)d1 * 16U) + ((seq_t)d2 * 4U) + (seq_t)d3;
    return value;
}
