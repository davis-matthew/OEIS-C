/*
 * A117084  Octagonal numbers for which both the sum and the product of the
 *          digits is also an octagonal number.
 * https://oeis.org/A117084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   digit_product, digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A117084:
   octagonal numbers for which both the sum and the product of the
   decimal digits are also octagonal.  The function generates octagonal
   numbers in order and keeps a static cache of those that satisfy the
   condition, allowing random access to any index. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* Test whether m is an octagonal number.
   The definition is m = k(3k−2).  For m > 0 we check that
   1 + 3m is a perfect square r² with (1 + r) divisible by 3.
   0 is considered octagonal by definition. */
static bool is_octagonal(seq_t m)
{
    if (m == (seq_t)0) {
        return true;
    }
    /* s = 1 + 3m */
    seq_t s = (seq_t)1 + (seq_t)3 * m;
    seq_t r = floor_sqrt(s);
    if (r == (seq_t)0) {
        return false;
    }
    if (r * r != s) {
        return false;
    }
    return ((seq_t)1 + r) % (seq_t)3 == (seq_t)0;
}

seq_t a(idx_t n)
{
    /* Lazy cache of qualifying octagonal numbers */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Next octagonal index to test */
    static idx_t next_k = 0;

    /* Grow allocation if necessary to hold requested index */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 256 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* memory allocation failed */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate octagonal numbers until we have at least n+1 qualified terms */
    while (cache_len <= n) {
        /* Compute the next octagonal number: O_k = k*(3k−2) */
        seq_t k = (seq_t)next_k;
        seq_t oct = k * ((seq_t)3 * k - (seq_t)2);

        seq_t s = digit_sum(oct);
        seq_t p = digit_product(oct);

        if (is_octagonal(s) && is_octagonal(p)) {
            /* Ensure capacity for storing this term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = oct;
        }
        next_k++;
    }

    return cache[n];
}
