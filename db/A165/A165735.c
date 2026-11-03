/*
 * A165735  Surviving integers under the double-count Josephus problem (see
 *          A054995), modulo 3.
 * https://oeis.org/A165735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = J3(n) mod 3, where J3 is the survivor in the double‑count
 *            Josephus problem for skip 3, defined recursively by J3(1)=1,
 *            J3(2)=2, J3(3m) = J3(2m) + [(J3(2m)-1)/2], J3(3m+1) = 3m+1 if
 *            J3(2m+1)=1 J3(2m+1)+[J3(2m+1)/2]-2 otherwise, J3(3m+2) =
 *            J3(2m+1)+[J3(2m+1)/2]+1. (See OEIS A054995; the sequ...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Recursive computation of J3(n).  No memoisation is used – depth is
   O(log n) so it remains fast for all n up to at least 10⁵. */
static seq_t J3(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t m = n / 3;
    if (n % 3 == 0) {                      /* J3(3m) */
        seq_t v = J3(2 * m);
        return v + (v - (seq_t)1) / (seq_t)2;
    } else if (n % 3 == 1) {               /* J3(3m+1) */
        seq_t v = J3(2 * m + 1);
        if (v == (seq_t)1)
            return (seq_t)n;                /* 3m+1 == n */
        return v + v / (seq_t)2 - (seq_t)2;
    } else {                               /* n%3 == 2, J3(3m+2) */
        seq_t v = J3(2 * m + 1);
        return v + v / (seq_t)2 + (seq_t)1;
    }
}

/* Public OEIS sequence function */
seq_t a(idx_t n)
{
    seq_t j = J3(n);
    return j % (seq_t)3;
}
