/*
 * A050210  Largest denominator in unit fraction representation of triangle of
 *          numbers 1/2, 1/3, 2/3, 1/4, 2/4, ... as computed with greedy
 *          algorithm.
 * https://oeis.org/A050210
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 124, u32 n <= 440, u64 n <= 1370,
 *            u128 n <= 1370, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): largest denominator that occurs in the greedy Egyptian
   fraction representation of the n‑th entry of the triangle
   1/2, 1/3, 2/3, 1/4, 2/4, 3/4, … .
   Indexing starts at n=2 for the first entry (1/2). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

static inline seq_t egyptian_largest_denom(seq_t num, seq_t den) {
    seq_t largest = 0;
    /* work with a/ b  (a,b > 0) */
    seq_t a = num;
    seq_t b = den;
    while (a != 0) {
        /* d = ceil(b / a) */
        seq_t d = (b + a - 1) / a;
        if (d > largest) largest = d;

        /* remainder: a' = a*d - b ;  b' = b*d */
        seq_t new_a = a * d - b;
        seq_t new_b = b * d;
        if (new_a == 0) break;          /* exact match, done */

        /* reduce the fraction */
        seq_t g = gcd(new_a, new_b);
        new_a /= g;
        new_b /= g;

        a = new_a;
        b = new_b;
    }
    return largest;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;           /* indices below the offset are irrelevant */

    /* Find the row r such that T_{r-1} <= n-2 < T_r, where
       T_k = k*(k-1)/2 is the triangular number. */
    idx_t m = n - 2;               /* shift so that row 2 begins at 1 */
    idx_t r = 2;
    while ((unsigned __int128)r * (r - 1) / 2 <= m) r++;
    /* r is now the row number (>=2) */

    /* Number of entries before this row */
    idx_t prev = (unsigned __int128)(r - 1) * (r - 2) / 2;
    /* Position k within the row (1-based) */
    idx_t k = n - 1 - prev;        /* 1 <= k < r */

    /* The fraction is k / r.  Compute the largest denominator. */
    return egyptian_largest_denom((seq_t)k, (seq_t)r);
}
