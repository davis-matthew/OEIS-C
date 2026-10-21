/*
 * A206424  The number of 1's in row n of Pascal's Triangle (mod 3).
 * https://oeis.org/A206424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^{c1} * (3^{c2} + 1) / 2 * where c1 = number of base‑3
 *            digits equal to 1, * c2 = number of base‑3 digits equal to 2. *
 *            * This counts the number of k (0 ≤ k ≤ n) such that * C(n,k) ≡ 1
 *            (mod 3) using Lucas' theorem
 * Range:     u8 n <= 726, u16 n <= 9584, u32 n <= 9584, u64 n <= 9584,
 *            u128 n <= 9584, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Count digits equal to 1 and 2 in base 3 representation of n */
    idx_t tmp = n;
    unsigned c1 = 0, c2 = 0;
    while (tmp > 0) {
        unsigned digit = tmp % 3;
        if (digit == 1)
            ++c1;
        else if (digit == 2)
            ++c2;
        tmp /= 3;
    }

    /* Compute 3^c2 as seq_t */
    seq_t p3 = pow3(c2);
    /* (3^c2 + 1) is always even */
    seq_t sum = p3 + (seq_t)1;

    /* 2^c1 is a shift; sum is even, so division by 2 is exact */
    seq_t mult = sum * ((seq_t)1 << c1);
    return mult / 2;
}
