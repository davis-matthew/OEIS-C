/*
 * A044058  Numbers n such that string 2,0 occurs in the base 3 representation
 *          of n but not of n-1.
 * https://oeis.org/A044058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has20_in_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth element of OEIS A044058.
 * The sequence consists of the numbers n such that the base‑3
 * representation of n contains the adjacent digits 2 followed by 0,
 * but the base‑3 representation of n‑1 does not.
 * The implementation searches for numbers in increasing order
 * and memoises the results so that subsequent calls are fast. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has20_in_base3.h"

/* Static cache used by the sequence function. */
static seq_t *cache_terms = NULL;     /* array of found terms           */
static size_t cache_capacity = 0;     /* current malloced capacity      */
static idx_t cache_count = 0;         /* number of terms in the cache   */
static seq_t next_candidate = 0;      /* next number to test (exclusive)*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 – not used     */

    /* grow the cache until it contains at least n terms */
    while (cache_count < n) {
        seq_t cur = next_candidate + 1;   /* start at the first number to test */
        while (1) {
            if (has20_in_base3(cur) && !has20_in_base3(cur - 1)) {
                /* store the term */
                if (cache_count >= cache_capacity) {
                    size_t newcap = cache_capacity ? cache_capacity * 2 : 8;
                    while (newcap <= cache_count) newcap <<= 1;
                    cache_terms = (seq_t *)realloc(cache_terms,
                                                    newcap * sizeof(seq_t));
                    if (!cache_terms) abort();      /* allocation failed */
                    cache_capacity = newcap;
                }
                cache_terms[cache_count++] = cur;
                next_candidate = cur;              /* next search starts after this number */
                break;
            }
            cur++;                                /* next candidate */
        }
    }

    return cache_terms[n - 1];
}
