/*
 * A044346  Numbers n such that string 1,4 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n-th integer whose decimal representation contains
   the substring "14" but that of n–1 does not. Brute‑force search with memoisation.
   The function works for any seq_t/idx_t types (uint8_t…uint64_t or an
   arbitrary‑precision integer class). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_cap = 0;     /* current allocated capacity */
static idx_t  cache_len = 0;     /* number of terms currently cached */
static seq_t  last_candidate = 0;/* last integer examined */

/* Return true iff the base‑10 representation of v contains the
   substring "14". */
static bool has14(seq_t v)
{
    if (v < 14) {
        return false;
    }
    seq_t tmp = v;
    int digits[200];
    size_t len = 0;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp = tmp / 10;
    }
    if (len < 2) {
        return false;
    }
    for (int i = (int)len - 1; i >= 1; --i) {
        if (digits[i] == 1 && digits[i - 1] == 4) {
            return true;
        }
    }
    return false;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined below the offset */
    }
    /* Extend cache until it contains the n-th term */
    while (cache_len < n) {
        ++last_candidate;
        if (has14(last_candidate) && !has14(last_candidate - 1)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real program inspect errno or handle out‑of‑memory. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_candidate;
        }
    }
    return cache[n - 1];
}
