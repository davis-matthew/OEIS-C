/*
 * A055239  Numbers which are not divisible by any of their digits in at least
 *          one base.
 * https://oeis.org/A055239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   ok_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A055239 by brute-force search.
   Numbers > 120 are known to belong to the sequence,
   so they are accepted automatically.  For numbers
   up to 120 the property is checked by exhaustive
   search over all bases.
   The function keeps a static array of already
   found terms and extends it lazily as needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/ok_small.h"

/* Helper: integer square root (floor) */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Sequence generation */
seq_t a(idx_t n) {
    static seq_t *terms = NULL;          /* stored sequence terms */
    static idx_t terms_len = 0;          /* how many terms stored */
    static uint64_t next_candidate = 11; /* next integer to test */
    static size_t terms_cap = 0;         /* current capacity of terms[] */

    /* Initialize on first call */
    if (terms == NULL) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Extend sequence until we have at least n terms */
    while (terms_len < n) {
        uint64_t m = next_candidate;
        if (ok_small(m)) {
            /* store term */
            if (terms_len >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    /* Return the requested term (1‑based index) */
    return terms[n - 1];
}
