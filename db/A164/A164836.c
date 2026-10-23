/*
 * A164836  a(n) is the number such that every k-digit substring ( k <= n )
 *          taken from the right end, is divisible by k.
 * https://oeis.org/A164836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164836:
 * a(n) is the n‑th smallest nonnegative integer such that for every
 * k (1 ≤ k ≤ number of decimal digits) the last k digits of a(n) are
 * divisible by k. This implementation searches sequentially and caches
 * the results so that arbitrary order of calls is supported. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static bool good_candidate(seq_t x)
{
    if (x == 0)
        return true;

    /* determine number of decimal digits */
    int digits = 0;
    seq_t tmp = x;
    do {
        digits++;
        tmp /= 10;
    } while (tmp != 0);

    seq_t pow10 = 10;
    for (int k = 1; k <= digits; ++k) {
        seq_t mod = x % pow10;          /* last k digits */
        if (mod % k != 0)
            return false;
        pow10 *= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0, len = 0;
    static seq_t cand   = 0;          /* next candidate to test */

    /* generate terms until we have at least n of them */
    while (len < n) {
        if (good_candidate(cand)) {
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);      /* out of memory */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;
        }
        ++cand;                       /* move to next integer */
    }
    return cache[n - 1];
}
