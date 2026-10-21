/*
 * A223456  Composite numbers whose number of proper divisors has a prime
 *          number of proper divisors.
 * https://oeis.org/A223456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A223456 (Composite numbers whose number of proper divisors * has
 *            a prime number of proper divisors - computed by brute‑force
 *            search)
 * Range:     u8 n <= 18, u16 n <= 9373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"                /* bool is_prime(idx_t x); */
#include "../utils/divisor_count_u64.h"      /* idx_t divisor_count_u64(uint64_t n); */

/* Check the defining property of a candidate number. */
static bool has_property(uint64_t x)
{
    /* Reject primes and numbers <= 1. */
    if (x < 4 || is_prime((idx_t)x))
        return false;

    /* Number of ALL divisors of x. */
    idx_t tau = divisor_count_u64(x);

    /* Proper divisors count of x. */
    idx_t prop1 = tau - 1;
    if (prop1 <= 1)
        return false;                     /* should not occur for composites */

    /* Proper divisors count of that count. */
    idx_t tau_prop1 = divisor_count_u64(prop1);
    idx_t prop2 = tau_prop1 - 1;
    if (prop2 <= 1)
        return false;

    /* The second‑stage proper divisor count must be prime. */
    return is_prime((idx_t)prop2);
}

/* Cache for the sequence values and the next candidate integer. */
static seq_t *terms      = NULL;        /* dynamic array of seq_t values */
static size_t terms_cap  = 0;           /* allocated capacity */
static size_t terms_size = 0;           /* number of values stored */
static uint64_t next_candidate = 4;     /* start searching from 4 */

/* Return the n-th term of A223456 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */

    /* Grow the cache if needed. */
    if (terms_size < (size_t)n) {
        size_t new_cap = terms_cap ? terms_cap : 4;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        if (new_cap > terms_cap) {
            seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);                /* allocation failed */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* Find additional terms until we reach n. */
        while (terms_size < (size_t)n) {
            uint64_t cand = next_candidate;
            while (!has_property(cand))
                ++cand;
            terms[terms_size++] = (seq_t)cand;
            next_candidate = cand + 1;
        }
    }
    return terms[n - 1];
}
