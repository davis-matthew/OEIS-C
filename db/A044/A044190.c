/*
 * A044190  Numbers n such that string 0,3 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers n such that the base‑8 representation of n
   contains the substring "03" but the representation of n-1 does not.
   The function returns the n‑th such integer. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Return true if the base‑8 representation of x contains the
   adjacent digits 0 followed by 3 (with 0 being more significant). */
static bool has_03(seq_t x)
{
    /* Temporary buffer for the octal digits, least‑significant first. */
    unsigned char digits[128];
    size_t len = 0;
    seq_t tmp = x;
    if (tmp == (seq_t)0)             /* 0 has no '03' substring */
        return false;

    while (tmp > (seq_t)0) {
        seq_t rem = tmp % (seq_t)8;
        digits[len++] = (unsigned char)rem;
        tmp /= (seq_t)8;
    }
    /* A pattern 0 (more significant) followed by 3 (less significant)
       corresponds to digits[i] == 3 and digits[i+1] == 0 in the
       least‑significant‑first array. */
    for (size_t i = 0; i + 1 < len; ++i) {
        if (digits[i] == (unsigned char)3 &&
            digits[i + 1] == (unsigned char)0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_to_test = 1;          /* next integer to examine     */
    static bool prev_has = false;           /* has_03(next_to_test-1) ? */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Ensure enough storage for the cache. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate numbers until the n‑th candidate is found. */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_to_test;
        bool cur_has = has_03(val);
        if (cur_has && !prev_has) {
            cache[cache_len++] = val;
        }
        prev_has = cur_has;
        ++next_to_test;
    }
    return cache[(size_t)n - 1];
}
