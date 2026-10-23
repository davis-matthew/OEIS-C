/*
 * A176948  a(n) is the smallest solution x to A176774(x)=n; a(n)=0 if this
 *          equation has no solution.
 * https://oeis.org/A176948
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 5797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176948: a(n) = smallest x such that x is n-gonal but not k-gonal
 * for any 3 <= k < n.  a(n) = 0 if no such x (only n=6). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* Test whether x is a k-gonal number (k >= 3).  Uses the quadratic
 * formula: 2x = (k-2)m^2 - (k-4)m.  The discriminant must be a perfect
 * square and the resulting m must be an integer >= 1. */
static inline bool is_k_gonal(seq_t x, idx_t k)
{
    seq_t k_seq = (seq_t)k;
    seq_t a = k_seq - (seq_t)2;          /* k-2 */
    seq_t b = k_seq - (seq_t)4;          /* k-4 */

    /* discriminant D = b^2 + 8*a*x */
    seq_t D = b * b + (seq_t)8 * a * x;
    seq_t s = floor_sqrt(D);
    if (s * s != D) return false;        /* not a perfect square */

    seq_t numerator = b + s;
    seq_t denom = (seq_t)2 * a;
    if (numerator % denom != 0) return false; /* m not integer */

    seq_t m = numerator / denom;
    return m >= (seq_t)1;                /* m must be >= 1 */
}

/* Main sequence function.  The offset is 3. */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* indices before offset are unused */
    if (n == 6) return (seq_t)0;         /* a(6) = 0 by definition */

    /* For prime n the smallest solution is n itself. */
    if (is_prime(n)) return (seq_t)n;

    /* Search for the minimal n-gonal number that is not lower-gonal. */
    for (idx_t m = 1; ; ++m)
    {
        seq_t m_s = (seq_t)m;
        seq_t n_s = (seq_t)n;

        /* Compute the m-th n-gonal number:
         * P(n,m) = ((n-2)m^2 - (n-4)m)/2
         */
        seq_t num = (n_s - (seq_t)2) * m_s * m_s
                    - (n_s - (seq_t)4) * m_s;
        seq_t x = num / (seq_t)2;
        if (x == (seq_t)0) continue;          /* skip trivial zero */

        /* Ensure x is not k-gonal for any 3 <= k < n. */
        bool ok = true;
        for (idx_t k = 3; k < n; ++k)
        {
            if (is_k_gonal(x, k))
            {
                ok = false;
                break;
            }
        }
        if (ok) return x;
        /* Theoretically this loop always terminates for n != 6. */
    }
}
