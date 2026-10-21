/*
 * A076786  Squarefree numbers such that in decimal representation all their
 *          prefixes are also squarefree.
 * https://oeis.org/A076786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A076786.
   Definition: square‑free numbers such that every decimal prefix is also square‑free.
   For n ≤ 0 returns 0.
   The function keeps a static cache so any order of calls is handled efficiently. */
   
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree_uint64.h"

/* Static cache of previously found terms */
static seq_t *terms = NULL;          /* Array of terms (seq_t) */
static idx_t terms_cnt = 0;          /* Number of terms stored so far */
static idx_t terms_cap = 0;          /* Current capacity of the terms array */

/* Helper: grow the terms array if needed */
static void grow_terms(void)
{
    if (terms_cnt < terms_cap) return;
    idx_t new_cap = terms_cap ? terms_cap * 2 : 32;
    seq_t *new_arr = (seq_t *)realloc(terms, (size_t)new_cap * sizeof(seq_t));
    if (!new_arr) { /* Allocation failed, keep the old array size to avoid crashes */
        return;
    }
    terms = new_arr;
    terms_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Guard against underflow */
    
    /* Ensure we have computed terms up to index n-1 */
    if (terms_cnt < n) {
        /* Start searching from 1 if cache empty, else from value after last cached term */
        uint64_t candidate = terms_cnt ? (uint64_t)terms[terms_cnt - 1] + 1 : 1;
        while (terms_cnt < n) {
            bool ok = true;
            uint64_t t = candidate;
            /* Test all decimal prefixes of candidate */
            while (t > 0) {
                if (!is_squarefree_uint64(t)) {
                    ok = false;
                    break;
                }
                t /= 10;
            }
            if (ok) {
                grow_terms();
                terms[terms_cnt] = (seq_t)candidate;
                terms_cnt++;
            }
            candidate++;
        }
    }
    return terms[n - 1];
}
