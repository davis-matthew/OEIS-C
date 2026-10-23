/*
 * A130335  Smallest k > 0 such that gcd(n*(n+1)/2, (n+k)*(n+k+1)/2) = 1.
 * https://oeis.org/A130335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k>0 : gcd(T(n), T(n+k)) = 1} , where T(m) = m*(m+1)/2
 *            is the m‑th triangular number. For all n >= 1, the function
 *            returns the smallest positive k such that T(n) and T(n+k) are
 *            coprime
 * Range:     u8 n <= 20, u16 n <= 2121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* outside OEIS offset, safe guard */

    /* Pre‑compute the triangular number for n. */
    seq_t t1 = (seq_t)n * ((seq_t)n + 1) / 2;

    for (idx_t k = 1;; ++k) {
        idx_t m = n + k;
        seq_t t2 = (seq_t)m * ((seq_t)m + 1) / 2;
        if (gcd(t1, t2) == 1)
            return (seq_t)k;
    }
}
