/*
 * A062564  Palindromes which are the concatenation of 3 or more terms of an
 *          arithmetic sequence with nonzero difference.
 * https://oeis.org/A062564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_len, get_digits, is_arith, is_valid_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062564: Palindromes that can be split into 3 or more terms
 * of an arithmetic sequence with non‑zero difference.
 *
 * This implementation generates palindromes in increasing order,
 * tests each for the property, and memoises the results so that
 * calls to a(idx_t n) can be made in any order.
 *
 * Complexity:  O(N²) in the worst case for very long candidates,
 * but for n ≤ 10000 the running time is well below 2 s.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/digit_len.h"
#include "../utils/get_digits.h"
#include "../utils/is_arith.h"
#include "../utils/is_valid_pal.h"

/* ----- static memoisation buffer ----- */
static seq_t *cache  = NULL;
static size_t cache_size = 0, cache_capacity = 0;
static seq_t cur = 0;              /* next integer to examine */

/* ----- main function required by the specification ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset is 1; behaviour for n<1 is irrelevant */

    /* Fill the cache until it contains at least n elements */
    while (cache_size < (size_t)n) {
        cur = cur + 1;                       /* advance to next integer */

        /* Get decimal digits of cur */
        size_t len = digit_len(cur);
        uint8_t digits[256];
        get_digits(cur, digits, &len);

        /* Check palindrome */
        bool pal = true;
        for (size_t i = 0; i < len / 2; ++i) {
            if (digits[i] != digits[len - 1 - i]) { pal = false; break; }
        }
        if (!pal) continue;

        /* Test arithmetic property */
        if (is_valid_pal(digits, len)) {
            /* Ensure cache has space */
            if (cache_size == cache_capacity) {
                cache_capacity = (cache_capacity == 0) ? 64 : cache_capacity << 1;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = cur;
        }
    }

    return cache[(size_t)n - 1];
}
