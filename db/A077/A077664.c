/*
 * A077664  Triangle in which the n-th row contains n smallest numbers greater
 *          than n and coprime to n.
 * https://oeis.org/A077664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 1822, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A077664
   a(n) is the (k)-th integer greater than r that is coprime to r,
   where r is the unique integer satisfying r(r+1)/2 ≥ n,
   and k = n − r(r−1)/2 − 1 (zero‑based). */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Undefined for n = 0; return 0 as a safeguard. */
    if (n == 0) return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((r + 1) * r / 2 < n) r++;

    /* Number of terms before row r. */
    idx_t prev = r * (r - 1) / 2;
    /* Zero‑based position within the current row. */
    idx_t k = n - prev - 1;

    /* Scan for the k‑th coprime greater than r. */
    idx_t cur = r + 1;   /* Integer type large enough for all checks. */
    idx_t count = 0;
    for (;; ++cur) {
        if (gcd((seq_t)r, (seq_t)cur) == (seq_t)1) {
            if (count == k) return (seq_t)cur;
            ++count;
        }
    }
}
