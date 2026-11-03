/*
 * A035127  Squares which when digits are rotated left once remain square.
 * https://oeis.org/A035127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   ipow_i32, mem, num_digits
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

#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    term_cache = mem_resize(seq_t, term_cache, new_cap);
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
        int digits = num_digits(s);

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
