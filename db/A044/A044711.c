/*
 * A044711  Numbers n such that string 8,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044711: numbers n such that the base‑9 representation of n contains
   the substring "87" but the base‑9 representation of n+1 does not.
   The function builds the sequence lazily and memoises the results. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number written in base 9 contains the substring 8,7. */
static bool contains_87(seq_t n)
{
    unsigned char digits[128];
    size_t len = 0;

    seq_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 9);
        tmp /= 9;
    }

    if (len < 2) return false;

    for (size_t i = 0; i + 1 < len; ++i) {
        if (digits[i] == 7 && digits[i + 1] == 8)
            return true;
    }
    return false;
}

/* Append a value to a dynamic array, resizing as needed. */
static void append_to_cache(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    ++(*len);
}

/* Sequence generator function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;  /* start searching at 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t x = next_candidate++;
        if (contains_87(x)) {
            seq_t xp1 = x + 1;
            if (!contains_87(xp1)) {
                append_to_cache(&cache, &cache_len, &cache_cap, x);
            }
        }
    }

    return cache[(size_t)n - 1];
}
