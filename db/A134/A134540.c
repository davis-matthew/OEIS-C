/*
 * A134540  Triangle read by rows: T(n,k) = Sum_{d|n, d>=k} moebius(n/d).
 * https://oeis.org/A134540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the value of the Triangle T(r,k) read by rows, where
 *            T(r,k) = Σ_{m|r, m ≤ r/k} μ(m) with μ the Möbius function. The
 *            index n (offset 1) is mapped to the pair (r,k) by row‐major
 *            ordering of the triangle
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dependencies:  seq_t and idx_t types are supplied by the build.
    mobius(idx_t x) returns μ(x) as seq_t.                           */
#include "../utils/mobius.h"
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* not used, but safe */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt((long double)8.0 * n + 1.0) - 1.0) / 2.0);
    /* adjust for possible rounding down */
    while ((__int128)r * (r + 1) / 2 < n) ++r;

    /* column k: position within row r */
    idx_t k = n - (r - 1) * r / 2;

    /* compute the triangle entry T(r,k) = Σ_{m|r, m ≤ r/k} μ(m) */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= r; ++d)
    {
        if (r % d == 0)
        {
            idx_t m1 = d;
            if (m1 <= r / k) sum += mobius(m1);
            idx_t m2 = r / d;
            if (m2 != m1 && m2 <= r / k) sum += mobius(m2);
        }
    }
    return sum;
}
