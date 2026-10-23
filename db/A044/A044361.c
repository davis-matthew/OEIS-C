/*
 * A044361  Numbers n such that string 2,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has29, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044361 – numbers n such that decimal string "29" occurs in n
   but not in n‑1.
   The function computes the n‑th term by scanning integers in order,
   remembering where the last computed term ended. */
       
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has29.h"

static seq_t *cache = NULL;          /* stored sequence terms        */
static size_t cache_cap = 0;         /* allocated capacity of cache  */
static size_t cache_len = 0;         /* number of terms stored       */
static idx_t  max_checked = 0;       /* largest integer examined so far */

/* Extend the cached sequence until it contains at least n terms.  */
static void ensure_terms(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Grow the cache if it hasn't been allocated yet. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
    }

    bool prev_has29 = false;
    if (max_checked > 0)
        prev_has29 = has29((seq_t)max_checked);

    for (idx_t cand = max_checked + 1; ; ++cand) {
        seq_t val = (seq_t)cand;
        bool curr_has29 = has29(val);

        if (curr_has29 && !prev_has29) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
            if (cache_len >= (size_t)n) {
                max_checked = cand;
                return;
            }
        }
        prev_has29 = curr_has29;
    }
}

/* External linkage: returns a(n) for n >= 1.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 but safe */

    ensure_terms(n);
    return cache[n - 1];
}
