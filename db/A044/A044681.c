/*
 * A044681  Numbers n such that string 5,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer n such that the base‑9
 *            representation of n contains the substring “54” while the base‑9
 *            representation of n+1 does not. Brute‑force enumeration with
 *            memoisation
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_substring54
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_substring54.h"

/* A static cache of previously generated terms.  Size is chosen to
   comfortably hold the required number of terms for the tests.       */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t candidate = 0;

/* Return the nth term (1‑based) of A044681.        */
seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* undefined for 0, but safe */

    /* Generate terms until we have at least n entries.                    */
    while (cache_len < n) {
        ++candidate;                         /* next integer to test     */
        if (has_substring54(candidate) &&
            !has_substring54(candidate + 1)) {
            if (cache_len < CACHE_CAP)
                cache[cache_len++] = candidate;
        }
    }
    return cache[n - 1];
}
