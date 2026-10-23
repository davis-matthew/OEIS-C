/*
 * A126247  a(n) is the number of triangular numbers, from among (T(1), T(2),
 *          T(3), ..., T(n)), which are coprime to T(n), where T(n) = n(n+1)/2
 *          is the n-th triangular number.
 * https://oeis.org/A126247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 1 ≤ k ≤ n : gcd(T(k), T(n)) = 1 }
 * Range:     u8 n <= 22, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where T(m) = m*(m+1)/2.  The function counts triangular numbers
   up to T(n) that are coprime with T(n). */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute T(n).  Use seq_t arithmetic to avoid overflow. */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t tk = (seq_t)k * ((seq_t)k + 1) / 2;
        if (gcd(tk, tn) == 1)
            ++count;
    }

    return count;
}
