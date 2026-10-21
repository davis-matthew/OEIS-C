/*
 * A070027  Prime numbers whose initial, all intermediate and final iterated
 *          sums of digits are primes.
 * https://oeis.org/A070027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth prime p such that p and all its iterated digit sums
 *            are prime
 * Range:     u8 n <= 22, u16 n <= 1178, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_sum.h"

static seq_t *term_cache = NULL;
static idx_t term_count = 0;
static seq_t next_candidate = 2;            /* start checking from 2 */

/* Check that the given prime p and all digit sums along the orbit are primes. */
static bool is_term_prime(seq_t p) {
    seq_t x = p;
    while (x >= 10) {
        seq_t s = digit_sum(x);
        if (!isPrimeGeneric(s))
            return false;
        x = s;
    }
    return true;
}

/* Append a new term to the cache, extending the storage as needed. */
static void add_term(seq_t val) {
    static size_t cap = 0;
    if (term_count == cap) {
        cap = cap ? cap << 1 : 8;
        term_cache = (seq_t *)realloc(term_cache, cap * sizeof(seq_t));
        if (!term_cache) abort();  /* allocation failure */
    }
    term_cache[term_count++] = val;
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;            /* offset is 1 – not used in the test */
    while (term_count < n) {
        if (isPrimeGeneric(next_candidate) && is_term_prime(next_candidate))
            add_term(next_candidate);
        ++next_candidate;
    }
    return term_cache[n - 1];
}
