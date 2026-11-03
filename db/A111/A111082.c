/*
 * A111082  Numbers n such that 3*n^2 + 49 is prime.
 * https://oeis.org/A111082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111082: numbers n such that 3*n^2 + 49 is prime.
   The implementation keeps a cache of terms and expands it by
   testing successive even candidates for primality.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Helper: 3*c^2 + 49 as uint64_t (fits for all requested n). */
static inline uint64_t poly_val(uint64_t c)
{
    unsigned __int128 v = (unsigned __int128)3 * c * c + 49;
    return (uint64_t)v;
}

/* Cache of discovered terms. */
static seq_t *terms   = NULL;   /* array of found n's */
static idx_t  term_cnt = 0;     /* number of terms stored */
static idx_t  term_cap = 0;     /* current capacity of the array */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, so n<1 is undefined */

    /* Ensure the cache is large enough. */
    if (terms == NULL) {
        term_cap = 1024;
        terms = mem_new(seq_t, term_cap);
    } else if (term_cnt >= term_cap) {
        term_cap <<= 1;
        terms = mem_resize(seq_t, terms, term_cap);
    }

    /* Generate terms until we have at least n of them. */
    uint64_t candidate = 2;
    if (term_cnt > 0) {
        /* start from the last found term + 2 (skip odds) */
        candidate = (uint64_t)terms[term_cnt - 1] + 2;
        if (candidate < 2) candidate = 2;
    }

    while (term_cnt < n) {
        uint64_t v = poly_val(candidate);
        if (is_prime_u64(v)) {
            terms[term_cnt++] = (seq_t)candidate;
        }
        candidate += 2;   /* only even candidates need testing */
    }

    return terms[n - 1];   /* a(1) is the first term */
}
