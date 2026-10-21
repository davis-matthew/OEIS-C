/*
 * A097619  Numbers having more prime factors than each of their neighbors.
 * https://oeis.org/A097619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   omega_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer m such that
   Ω(m) > Ω(m-1) and Ω(m) > Ω(m+1),
   where Ω(m) is the total number of prime factors of m
   (counted with multiplicity).  The function builds the
   sequence incrementally and caches the results so that
   calls in any order are fast. */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega_uint64.h"

/* ----------  cache for the sequence  ---------- */
static seq_t *term_cache = NULL;
static size_t   term_cap   = 0;
static idx_t    term_count = 0;

/* Ensure the cache can hold at least 'needed' terms.  */
static void ensure_cache(size_t needed)
{
    if (needed <= term_cap) return;
    size_t new_cap = term_cap ? (term_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(term_cache, new_cap * sizeof(seq_t));
    term_cache = (seq_t *)tmp;
    term_cap   = new_cap;
}

/* ----------  scanning for terms  ---------- */
static uint64_t last_checked = 1;   /* last integer examined (as uint64_t) */

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */

    while (term_count < n) {
        ++last_checked;                      /* next candidate number */
        if (last_checked <= 1) continue;     /* skip 1 (never qualifies) */

        uint64_t om  = omega_uint64(last_checked);
        uint64_t omp = omega_uint64(last_checked - 1);
        uint64_t omn = omega_uint64(last_checked + 1);

        if (om > omp && om > omn) {           /* qualifies as a term */
            ensure_cache(term_count + 1);
            term_cache[term_count++] = (seq_t)last_checked;
        }
    }
    return term_cache[n - 1];
}
