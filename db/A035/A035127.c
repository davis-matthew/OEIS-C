/*
 * A035127  Squares which when digits are rotated left once remain square.
 * https://oeis.org/A035127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Scan the sequence of all squares s = k^2 in increasing order.
   For each s compute its decimal digits, rotate the digits left once
   (d2 d3 … dk d1).  If the rotated number does not begin with a 0
   (i.e. the second digit of s is non‑zero) and the rotated number is
   again a perfect square, then s is recorded as a term of A035127.
   The resulting values are cached so that arbitrary calls to a(n)
   return the correct term.  This method is O(N) in the number of
   checked squares. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/pow10.h"        /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"    /* int digit_len(seq_t x); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

static seq_t *term_cache = NULL;     /* dynamic array of found terms */
static size_t cache_cap   = 0;
static size_t cache_len   = 0;
static idx_t  last_checked_k = 0;    /* largest k that has been examined */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap  = new_cap;
}

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Continue searching squares until we have at least n terms cached. */
    while (cache_len < n) {
        last_checked_k++;                     /* next integer root to test */
        seq_t s = (seq_t)last_checked_k * last_checked_k;

        /* Number of decimal digits of s.  digit_len is efficient. */
        int digits = digit_len(s);

        /* Exclude squares whose left rotation would have a leading zero.
           That happens when the second most significant digit is 0. */
        if (digits >= 2) {
            seq_t p10_digitsm2 = pow10((idx_t)(digits - 2));
            /* d2 = (s / 10^(digits-2)) % 10 */
            seq_t d2 = (s / p10_digitsm2) % 10;
            if (d2 == 0) continue;   /* leading zero would appear */
        }

        /* Compute the left‑rotated number.
           Let d1 be the most significant digit of s. */
        seq_t p10_digitsm1 = pow10((idx_t)(digits - 1));
        seq_t d1 = s / p10_digitsm1;           /* most significant digit  */
        seq_t rotated = ((s % p10_digitsm1) * 10) + d1;

        /* Check whether the rotated number is a perfect square. */
        seq_t rroot = floor_sqrt(rotated);
        if (rroot * rroot == rotated) {
            ensure_cache_cap(cache_len + 1);
            term_cache[cache_len++] = s;
        }
    }

    return term_cache[n - 1];
}
