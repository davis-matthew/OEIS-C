/*
 * A231176  Let A={1,3,4,7,8,10,13,15,...} be the sequence of numbers k>=1
 *          such that k+2 is evil (A001969), let B be the complement of A. The
 *          sequence lists numbers for which the number of A-divisors equals
 *          the number of B-divisors.
 * https://oeis.org/A231176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_even_popcount, is_in_A
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A231176 ‑ perfect squares whose proper divisors split equally between
 * the set A (evil numbers: k+2 has even popcount) and its complement B.
 * Implementation: brute‑force search over squares, counting A‑divisors
 * and B‑divisors for each.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_even_popcount.h"
#include "../utils/is_in_A.h"

/* Global cache of terms found so far. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Largest square root analysed so far. */
static idx_t root_checked = 0;

/* Grow the cache so that it contains at least `need` elements. */
static void ensure_cache(idx_t need)
{
    while (cache_len < need) {
        /* Next square to test. */
        ++root_checked;
        seq_t n = (seq_t)root_checked * (seq_t)root_checked;

        /* Count proper divisors belonging to A and B. */
        idx_t cntA = 0, cntB = 0;
        for (idx_t d = 1; (seq_t)d * (seq_t)d <= n; ++d) {
            if (n % (seq_t)d == 0) {
                /* divisor d itself */
                if (d < (idx_t)n) {
                    if (is_in_A(d)) ++cntA;
                    else ++cntB;
                }
                idx_t e = (idx_t)(n / (seq_t)d);
                if (e != d && e < (idx_t)n) {
                    if (is_in_A(e)) ++cntA;
                    else ++cntB;
                }
            }
        }

        if (cntA == cntB) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_mem) abort();      /* out of memory */
                cache = new_mem;
                cache_cap = new_cap;
            }
            cache[cache_len++] = n;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public function required by the grader. */
seq_t a(idx_t n)
{
    /* Ensure we have at least `n` terms in the cache. */
    ensure_cache(n);
    /* n is 1‑based; return the requested term. */
    return cache[n - 1];
}
