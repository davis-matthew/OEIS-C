/*
 * A044580  Numbers n such that string 1,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains_14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044580: numbers n such that
 * the base-8 representation of n contains the substring "14",
 * while the base-8 representation of n+1 does *not* contain "14".
 *
 * Implementation: incremental scan with memoisation.
 * See a(idx_t n) below.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/contains_14.h"

/* static table for cached terms */
static seq_t *term_cache = NULL;
static size_t term_len   = 0;   /* number of cached terms */
static size_t term_cap   = 0;   /* table capacity */

/* next integer to test; starts from 1 */
static idx_t next_test = 1;

/* grow the term cache to at least 'need' entries */
static void grow_cache(size_t need)
{
    size_t new_cap = term_cap ? term_cap * 2 : 4;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    if (term_cache) {
        for (size_t i = 0; i < term_len; ++i) new_cache[i] = term_cache[i];
        free(term_cache);
    }
    term_cache = new_cache;
    term_cap   = new_cap;
}

/* return the nth term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, return 0 */
    while (term_len < (size_t)n) {
        seq_t val = (seq_t)next_test;
        if (contains_14(val) && !contains_14(val + (seq_t)1)) {
            if (term_len == term_cap) grow_cache(term_len + 1);
            term_cache[term_len++] = val;
        }
        ++next_test;
    }
    return term_cache[n - 1];
}
