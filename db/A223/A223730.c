/*
 * A223730  Multiplicities for representations of positive numbers n as
 *          primitive sums of three nonzero squares.
 * https://oeis.org/A223730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223730: Primitive representations of n as a^2 + b^2 + c^2
 *  a(n) = number of distinct triples of positive integers (a,b,c)
 *  with gcd(a,b,c)=1 and a^2 + b^2 + c^2 = n.
 *  The algorithm searches all 1 ≤ a ≤ b ≤ c with a^2+b^2+c^2 = n
 *  and counts the primitive ones. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for n ≥ 1 only */

    seq_t count = 0;

    /* sqrt_n fits in idx_t (sqrt(2^64-1) < 2^32) */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);

    for (idx_t a = 1; a <= sqrt_n; ++a)
    {
        idx_t a_sq = a * a;
        if (a_sq > n) break;

        for (idx_t b = a; b <= sqrt_n; ++b)
        {
            idx_t b_sq = b * b;
            idx_t sum_ab = a_sq + b_sq;
            if (sum_ab > n) break;

            idx_t rem = n - sum_ab;
            /* rem ≤ 2^64-1, sqrt(rem) fits in idx_t */
            idx_t c = (idx_t)floor_sqrt((seq_t)rem);

            if (c < b) continue;          /* enforce a ≤ b ≤ c */

            if ((seq_t)c * (seq_t)c != (seq_t)rem) continue;

            seq_t g = gcd(gcd((seq_t)a, (seq_t)b), (seq_t)c);
            if (g == 1) ++count;
        }
    }

    return count;
}
