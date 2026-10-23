/*
 * A172981  Primes p such that 210*p+41 is also prime.
 * https://oeis.org/A172981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 32, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 210*p+41 is prime */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Cache for already found terms */
static seq_t *term_cache = NULL;
static size_t term_cap  = 0;
static size_t term_cnt  = 0;

/* Current prime candidate to test */
static seq_t next_candidate_p = 2;

/* Append a new term to the cache, expanding the array as needed */
static void cache_append(seq_t val)
{
    if (term_cnt >= term_cap) {
        term_cap = term_cap ? term_cap * 2 : 16;
        term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
        if (!term_cache) abort();
    }
    term_cache[term_cnt++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    /* Generate terms until we have at least n */
    while (term_cnt < n) {
        /* Scan forward until we find the next qualifying prime p */
        while (true) {
            if (is_prime_small(next_candidate_p)) {
                /* t = 210*p + 41 */
                seq_t cand = next_candidate_p * (seq_t)210 + (seq_t)41;
                if (is_prime_big(cand)) {
                    cache_append(next_candidate_p);
                    /* Move past the found prime for the next search */
                    next_candidate_p++;
                    break;             /* found a term; back to outer loop */
                }
            }
            next_candidate_p++;        /* try the next integer */
        }
    }

    return term_cache[n - 1];
}
