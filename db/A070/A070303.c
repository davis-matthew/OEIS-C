/*
 * A070303  Primes p such that the equation p^2*x^2==0 (mod p^2+x^2) has no
 *          solution.
 * https://oeis.org/A070303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 6500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th prime that is NOT of the form k^2 + 1.
   The sequence is A070303 in OEIS.  The implementation
   precomputes primes by a sieve up to a growing limit and
   stores the filtered primes in a static array.  It grows
   the sieve only when a larger n is requested. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper: integer square root for 32‑bit values. */
static uint32_t isqrt32(uint32_t n)
{
    uint32_t r = 0;
    while ((r + 1U) * (r + 1U) <= n) r++;
    return r;
}

/* Build the filtered prime list up to the given limit. */
static void rebuild_terms(uint32_t limit, seq_t **terms_out, size_t *count_out)
{
    /* Sieve of Eratosthenes. */
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return;
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= limit; j += i) is_comp[j] = true;
        }
    }

    /* Count primes that are NOT of the form k^2 + 1. */
    size_t cnt = 0;
    for (uint32_t x = 2; x <= limit; ++x) {
        if (!is_comp[x]) {
            uint32_t y = x - 1;
            uint32_t s = isqrt32(y);
            if (s * s != y) cnt++;
        }
    }

    seq_t *new_terms = (seq_t*)malloc(cnt * sizeof(seq_t));
    if (!new_terms) {
        free(is_comp);
        return;
    }

    size_t idx = 0;
    for (uint32_t x = 2; x <= limit; ++x) {
        if (!is_comp[x]) {
            uint32_t y = x - 1;
            uint32_t s = isqrt32(y);
            if (s * s != y) {
                new_terms[idx++] = (seq_t)x;
            }
        }
    }

    free(is_comp);
    *terms_out = new_terms;
    *count_out = cnt;
}

/* Return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_count = 0;
    static uint32_t current_limit = 0;

    if (n == 0) return 0;           /* Undefined, but safe. */

    /* Initial build if needed. */
    if (current_limit == 0) {
        /* Start with a reasonable limit: 400000 approximates > 30k primes. */
        current_limit = 400000;
        rebuild_terms(current_limit, &terms, &term_count);
    }

    /* Grow the limit until enough terms are available. */
    while (term_count < (size_t)n) {
        uint32_t new_limit = current_limit * 2;
        free(terms);                /* Release old terms. */
        rebuild_terms(new_limit, &terms, &term_count);
        current_limit = new_limit;
    }

    return terms[n - 1];
}
