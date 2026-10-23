/*
 * A043998  Numbers k such that 0 and 3 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A043998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reserve_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth term of OEIS A043998: numbers k such that the digits
 * 0 and 3 occur adjacent in the decimal representation of k but not of k+1.
 * The implementation scans integers, keeps a cache of discovered terms,
 * and extends the cache lazily on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reserve_cache.h"

/* Test whether a number contains the adjacent digit pair 03 or 30. */
static bool has_adjacent_03_30(seq_t x)
{
    if (x < (seq_t)10) return false;          /* less than two digits */
    seq_t prev = x % (seq_t)10;
    x      /= (seq_t)10;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)10;
        if ((prev == (seq_t)0 && cur == (seq_t)3) ||
            (prev == (seq_t)3 && cur == (seq_t)0))
            return true;
        prev = cur;
        x    /= (seq_t)10;
    }
    return false;
}

/* The function required by the benchmark harness.  It is memoised so that
 * subsequent calls for the same or smaller indices are answered instantly,
 * while larger indices extend the cache only as far as needed. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour for n<1 is irrelevant */

    static size_t count = 0;             /* number of terms currently cached */
    static size_t cap   = 0;
    static seq_t *cache = NULL;
    static seq_t search_start = (seq_t)0; /* next number to inspect */

    /* Extend the cache until it contains at least n terms. */
    while (count < n) {
        seq_t k = search_start;
        while (1) {
            if (has_adjacent_03_30(k) && !has_adjacent_03_30(k + (seq_t)1)) {
                if (count == cap) {                 /* grow cache */
                    size_t newcap = cap ? cap * 2 : 16;
                    if (newcap < count + 1) newcap = count + 1;
                    seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!newp) abort();
                    cache = newp;
                    cap   = newcap;
                }
                cache[count++] = k;
                search_start   = k + (seq_t)1;
                break;
            }
            ++k;
        }
    }

    return cache[n-1];
}
