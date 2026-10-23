/*
 * A044299  Numbers k such that the string 5,3 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the string 53 occurs in the base‑9 representation of k but not of k‑1. */
#include <stdlib.h>
#include <stdbool.h>

/* Helper: true if x (in base 9) contains the adjacent digit pair 5,3   */
static bool has53(seq_t x)
{
    if (x == 0)
        return false;
    seq_t digits[32];
    size_t n = 0;
    while (x > 0) {
        digits[n++] = x % 9;
        x = x / 9;
    }
    for (size_t i = 0; i + 1 < n; ++i) {
        if (digits[i + 1] == 5 && digits[i] == 3)
            return true;
    }
    return false;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 1;

    /* Grow cache as needed. */
    while (cache_size < (size_t)n) {
        /* Test the candidate number. */
        if (has53(next_k) && !has53(next_k - 1)) {
            if (cache_size == cache_cap) {
                size_t newcap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = next_k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
