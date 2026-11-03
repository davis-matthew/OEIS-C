/*
 * A048306  Numbers whose base-9 expansions, read from left to right, have run
 *          lengths that strictly increase.
 * https://oeis.org/A048306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth (0‑indexed) integer whose base‑9 expansion has
 *            strictly increasing run lengths. The sequence is computed by a
 *            simple brute‑force search
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether the base‑9 digits of x have strictly increasing run lengths. */
static bool increasing_run(seq_t x)
{
    if (x == 0)
        return true;                       /* single digit "0" is valid */

    unsigned char digits[64];
    size_t len = 0;
    seq_t t = x;
    while (t) {
        digits[len++] = (unsigned char)(t % 9);
        t /= 9;
    }

    /* Process runs from most significant to least significant digit. */
    int last_len     = 0;                     /* length of previous run   */
    unsigned char prev = digits[len - 1];     /* most significant digit   */
    size_t run_len = 1;                       /* current run length       */

    /* Iterate over the remaining digits. */
    for (size_t i = len - 1; i-- > 0; ) {
        unsigned char cur = digits[i];
        if (cur == prev) {
            ++run_len;
        } else {
            if ((size_t)run_len <= (size_t)last_len)
                return false;
            last_len = (int)run_len;
            prev     = cur;
            run_len  = 1;
        }
    }

    /* Final run check. */
    return (size_t)run_len > (size_t)last_len;
}

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   count     = 0;          /* how many terms cached */
    static idx_t   next      = 0;          /* next integer to test  */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = mem_resize(seq_t, NULL, cache_cap);
    }

    /* generate terms until we have at least n+1 of them */
    while (count <= n) {
        if (increasing_run((seq_t)next)) {
            if (count == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[count++] = (seq_t)next;
        }
        ++next;
    }

    return cache[n];
}
