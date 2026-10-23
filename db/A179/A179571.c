/*
 * A179571  Number of permutations of 1..n+4 with the number moved left
 *          exceeding the number moved right by n.
 * https://oeis.org/A179571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 3*n - 6)/2 + 2^(n+4) (valid for all n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 99,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the power-of-two part first. */
    seq_t p = pow2(n + 4);                 /* 2^(n+4) */

    /* Compute the triangular part in a signed 64‑bit intermediate
       to avoid unsigned wrap‑around for n = 1 (the only case giving a
       negative numerator). */
    long long tri_num = (long long)n * (long long)n
                      + 3LL * (long long)n
                      - 6LL;           /* n^2 + 3n - 6 */
    long long tri = tri_num / 2;         /* (n^2 + 3n - 6)/2 */

    /* Add the two parts safely, using subtraction when tri is negative. */
    if (tri >= 0)
        return p + (seq_t)tri;
    else
        return p - (seq_t)(-tri);
}
