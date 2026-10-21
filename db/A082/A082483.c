/*
 * A082483  Numbers k such that 1/(5-s(k)) is an integer where s(k) =
 *          Sum_{i=1..k} 1/2^floor(sqrt(i)).
 * https://oeis.org/A082483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that s(k) = 5 − 1/2^(n‑1), where s(k) =
 *            Σ_{i=1}^k 1/2^⌊√i⌋
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived algorithm: find smallest m with 2m+5 ≤ 2^{m-(n-1)}.
   Then t = 4m+6 − 2^{m-(n-1)} and a(n) = m^2 + t − 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but harmless */

    idx_t m = 1;
    for (;; ++m) {
        /* The exponent diff = m - (n-1) is non‑negative once m≥n-1.
           The required 2^{diff} is very small (≤ about 16 for all n≤10000). */
        if (m >= n-1) {
            unsigned int diff = (unsigned int)(m - (n - 1));
            unsigned long long rhs = 1ULL << diff;      /* 2^{diff} */
            if ((unsigned long long)(2*m + 5) <= rhs) { /* 2m+5 ≤ 2^{diff} */
                unsigned long long t = 4ull * (unsigned long long)m + 6ull - rhs;
                idx_t k = m * m + (idx_t)t - 1;
                return (seq_t)k;
            }
        }
        /* For m < n-1 the inequality cannot hold, so the loop simply
           increments m until the condition becomes true. */
    }
}
