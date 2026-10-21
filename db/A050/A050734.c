/*
 * A050734  Numbers of form 4^k (values of k see A050725) containing no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 4^k where k is the n-th exponent such that 4^k has no *
 *            consecutive equal decimal digits
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small, no_consecutive_equal_digits, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cached_terms = NULL;    /* dynamic array of found terms   */
static size_t cache_capacity = 0;      /* current array capacity         */
static idx_t term_count = 0;           /* number of terms stored         */
static seq_t current_val = 0;          /* 4^current_k                    */
static idx_t current_k = 0;            /* exponent of current_val         */

/* Ensure the cache has at least 'needed' slots. */
static void ensure_capacity(size_t needed)
{
    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
        while (new_cap < needed)
            new_cap *= 2;

        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (!tmp) abort();                /* out of memory */
        for (size_t i = 0; i < cache_capacity; ++i)
            tmp[i] = cached_terms[i];
        if (cached_terms)
            seq_free_array(cached_terms);
        cached_terms = tmp;
        cache_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        seq_t val;
        if (current_k == 0 && current_val == 0) {
            /* 4^0 = 1 */
            val = (seq_t)1;
        } else {
            val = mul_small(current_val, (unsigned int)4);
        }

        if (no_consecutive_equal_digits(val)) {
            ensure_capacity(term_count + 1);
            cached_terms[term_count++] = val;
        }

        current_val = val;
        current_k++;
    }

    return cached_terms[n - 1];
}
