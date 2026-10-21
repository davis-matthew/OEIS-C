/*
 * A093413  Largest number in row n of A093412.
 * https://oeis.org/A093413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest odd integer ≤ 2n–1 that is coprime to the odd
 *            part of n+1. (The OEIS definition uses the triangle A093412;
 *            this equivalent explicit formula yields the same values. Special
 *            cases: a(1)=1, a(2)=2.)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the two small special cases. */
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* Compute the odd part of n+1. */
    idx_t temp = n + 1;
    while ((temp & 1) == 0)          /* remove factors of 2 */
        temp >>= 1;
    seq_t d = (seq_t)temp;            /* odd part of n+1 */

    /* Start from the largest odd number ≤ 2n–1 and step down by 2 until
       an odd number coprime to d is found. */
    seq_t m = (seq_t)n + (seq_t)n - 1;/* 2*n – 1, already odd */
    const seq_t one = 1;
    while (gcd(m, d) != one)
        m -= 2;

    return m;
}
