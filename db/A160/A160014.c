/*
 * A160014  Generalized Clausen numbers (table read by antidiagonals).
 * https://oeis.org/A160014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 439, u32 n <= 3488, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generalized Clausen numbers read by antidiagonals (OEIS A160014)
   t(m,k) = ∏_{p prime, (p−k) | m} p.
   The index n is mapped to the pair (m,k) on the antidiagonal
   of the triangular array: (m,k) = (s−q, q) for
   the diagonal of length s+1 that contains n.
   The first diagonal (s=0) has the single element (0,0). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find diagonal s and position q within it */
    idx_t s   = 0;                /* diagonal index (0‑based)   */
    idx_t len = 1;                /* length of current diagonal */
    idx_t rem = n;                /* remaining index to skip   */
    while (rem >= len) {
        rem -= len;               /* skip whole diagonal      */
        ++s;                      /* advance to next diagonal */
        ++len;                    /* next diagonal is one longer */
    }

    /* Position inside diagonal:
       (m,k) = (s−rem, rem)  ⇔  (row,column)   */
    idx_t k = rem;
    idx_t m = s - rem;

    /* T(0,k) is defined as 1 */
    if (m == 0)
        return (seq_t)1;

    /* Product over primes p= d+k where d | m */
    seq_t prod = 1;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t p1 = d + k;
            if (p1 > 1 && is_prime(p1))
                prod *= (seq_t)p1;

            idx_t d2 = m / d;
            if (d2 != d) {
                idx_t p2 = d2 + k;
                if (p2 > 1 && is_prime(p2))
                    prod *= (seq_t)p2;
            }
        }
    }
    return prod;
}
