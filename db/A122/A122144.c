/*
 * A122144  Numbers k such that q(k) = M(k) where q(n) is the largest prime
 *          divisor of k and M(k) is the largest prime power divisor of k.
 * https://oeis.org/A122144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that largest prime
 *            divisor of k equals largest prime power divisor of k. The first
 *            24 terms are:
 *            2,3,5,6,7,10,11,13,14,15,17,19,20,21,22,23,26,28,29,30,31,33,34,35
 *            Christopher R. 2024
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"          /* largest prime divisor of a uint64_t */

static uint64_t *matches = NULL;  /* cached terms */
static size_t match_len = 0;      /* number of terms stored */
static size_t match_cap = 0;      /* capacity of matches buffer */
static uint64_t last_k = 1;       /* last k examined */

/* Return true iff k satisfies q(k) = M(k). */
static bool is_match(uint64_t k)
{
    uint64_t n = k;
    uint64_t max_prime = 0;
    uint64_t max_power = 0;

    while (n > 1) {
        /* p is the largest prime divisor of n at this point */
        uint64_t p = gpf(n);
        uint64_t power = 1;
        /* factor out all copies of p to get the largest power of p dividing n */
        while (n % p == 0) {
            n   /= p;
            power *= p;
        }
        /* the first (i.e. largest) prime encountered is the max_prime */
        if (max_prime == 0) max_prime = p;
        if (power > max_power) max_power = power;
    }

    return max_prime == max_power;
}

/* Ensure the array has space for at least 'required' additional entries. */
static void ensure_capacity(size_t required)
{
    if (matches == NULL) {
        match_cap = 16;
        matches = (uint64_t *)calloc(match_cap, sizeof(uint64_t));
    } else if (match_len + required > match_cap) {
        size_t new_cap = match_cap * 2;
        while (match_len + required > new_cap) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(matches, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* out of memory */
        matches = tmp;
        match_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* dummy return for index 0 */

    /* generate terms until we have at least n entries */
    while (match_len < (size_t)n) {
        ++last_k;
        if (is_match(last_k)) {
            ensure_capacity(1);
            matches[match_len++] = last_k;
        }
    }

    return (seq_t)matches[(size_t)n - 1];
}
