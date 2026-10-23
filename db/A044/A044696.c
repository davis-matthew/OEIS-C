/*
 * A044696  Numbers n such that string 7,1 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044696
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

/* Sequence A044696: numbers n such that the base‑9 representation of n
 * contains the string "71" but the base‑9 representation of n+1 does not.
 * The function a(n) returns the n‑th term (offset 1).  The algorithm
 * performs a simple brute‑force search, caching all found terms so
 * subsequent calls (in any order) reuse previously computed values.  */

#include <stdlib.h>
#include <stdbool.h>

/* True if the base‑9 representation of x contains the substring 71. */
static bool has71(seq_t x)
{
    unsigned char digits[256];
    size_t len = 0;

    if (x == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x != (seq_t)0) {
            digits[len++] = (unsigned char)(x % (seq_t)9);
            x = x / (seq_t)9;
        }
    }

    /* Scan reversed digits: digits[i]==7 and digits[i-1]==1. */
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == 7 && digits[i - 1] == 1)
            return true;
    }
    return false;
}

/* Cached found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
/* Next integer to test */
static seq_t next_val = (seq_t)1;

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_val = (seq_t)1;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        while (true) {
            seq_t cur = next_val;
            seq_t nxt = cur + (seq_t)1;

            if (has71(cur) && !has71(nxt)) {
                /* Store the qualifying number */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache,
                                             cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = cur;

                /* Next search starts from the following integer */
                next_val = nxt;
                break;
            }
            ++next_val; /* Advance to the next candidate */
        }
    }

    return cache[(size_t)n - 1];
}
