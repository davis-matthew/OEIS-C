/*
 * A145823  Squares of the form p1 - 1 where p1 is a lower twin prime.
 * https://oeis.org/A145823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k^2 where k^2 + 1 and k^2 + 3 are prime. Each term is the
 *            square of an integer k such that k^2+1 (the lower twin prime)
 *            and k^2+3 are both prime
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 11, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence on demand, caching found terms
   for subsequent calls.  A fixed size cache is used: the sequence
   required for testing contains far fewer than 20 000 terms, so this
   size is safe for all practical purposes. */

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* A static cache big enough for the expected range of n. */
    static seq_t cache[20000];
    static size_t len  = 0;   /* number of terms currently stored */
    static idx_t  k    = 2;   /* next integer to test (k = sqrt(term)) */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        seq_t k_sq = (seq_t)k * (seq_t)k;  /* k^2 */
        seq_t p1   = k_sq + 1;             /* lower twin prime candidate */
        seq_t p2   = p1 + 2;               /* upper twin prime candidate */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            assert(len < 20000);          /* safety guard */
            cache[len++] = k_sq;
        }
        ++k;
    }

    return cache[n - 1];
}
