/*
 * A118002  a(0) = 0. a(n) = a(n-1) + (largest integer <= n which is coprime
 *          to a(n-1)).
 * https://oeis.org/A118002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = a(n-1) + max{ m | 1 ≤ m ≤ n , gcd(a(n-1), m) =
 *            1 } for n ≥ 1
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the n-th term of OEIS sequence A118002.
   Works for any integer type seq_t and idx_t provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t cur = 0;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m = i;              /* candidate m starting from i */
        while (m > 0 && gcd(cur, m) != 1)
            --m;                  /* find largest m ≤ i coprime to cur */
        cur += m;                  /* a(i) = a(i-1) + m */
    }
    return cur;
}
