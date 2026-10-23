/*
 * A138172  Even n where d(n) < d(n+1), where d(n) = number of positive
 *          divisors of n.
 * https://oeis.org/A138172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 4855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A138172: even n such that d(n) < d(n+1) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/seq_alloc.h"   /* for seq_alloc, seq_free */

/* Cache state for the sequence */
static idx_t _a_limit = 0;                /* current sieve limit           */
static idx_t *_a_spf = NULL;              /* smallest prime factor array   */
static seq_t *_a_divs = NULL;             /* divisor–count array           */
static idx_t *_a_terms = NULL;            /* list of qualifying even n     */
static size_t  _a_term_cap = 0;           /* allocated capacity of _a_terms */
static size_t  _a_term_len = 0;           /* number of stored terms         */

/* Helper: grow the terms array if necessary */
static void _grow_terms(void)
{
    if (_a_term_len >= _a_term_cap) {
        size_t new_cap = _a_term_cap ? _a_term_cap << 1 : 16;
        idx_t *tmp = (idx_t *)realloc(_a_terms, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        _a_terms = tmp;
        _a_term_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have at least n qualifying terms */
    while (_a_term_len < (size_t)n) {
        /* Determine a new, larger sieve limit */
        idx_t new_limit = (_a_limit == 0) ? 1000
                                          : (_a_limit << 1) + 1000;
        if (new_limit <= _a_limit) new_limit = _a_limit + 1000; /* safety */

        /* Rebuild the SPF table for the new limit */
        if (_a_spf) free(_a_spf);
        build_spf(new_limit, &_a_spf);
        if (!_a_spf) exit(EXIT_FAILURE);   /* build failure */

        /* Allocate (or re‑allocate) the divisor‑count array */
        size_t new_cap = (size_t)(new_limit + 1);
        seq_t *tmp = seq_alloc(new_cap);
        if (!tmp) exit(EXIT_FAILURE);       /* out of memory */
        _a_divs = tmp;
        /* Initialise the first two values */
        _a_divs[0] = 0;
        _a_divs[1] = 1;

        /* Compute divisor counts for all numbers up to new_limit */
        for (idx_t i = 2; i <= new_limit; ++i) {
            /* factor i with its smallest prime factor */
            idx_t p = _a_spf[i];
            idx_t m = i / p;
            idx_t e = 1;
            while (m % p == 0) { m /= p; ++e; }

            /* d(i) = d(m) * (e+1) */
            _a_divs[i] = _a_divs[m] * (seq_t)(e + 1);
        }

        /* Scan for new qualifying even numbers */
        for (idx_t i = _a_limit + 2; i <= new_limit; ++i) {
            idx_t e = i - 1;           /* the even number to test */
            if (((e & 1) == 0) && (e > _a_limit) &&
                (_a_divs[e] < _a_divs[i])) {
                _grow_terms();
                _a_terms[_a_term_len++] = e;
            }
        }

        /* Update the limit for the next round */
        _a_limit = new_limit;
    }

    return (seq_t)_a_terms[n - 1];
}
