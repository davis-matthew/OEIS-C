/*
 * A121934  Smallest positive number m such that m == i (mod i+1) for all
 *          1<=i<=n.
 * https://oeis.org/A121934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = lcm(2,3,…,n) – 1 for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case n = 1, where the product is empty. */
    if (n <= 1)
        return (seq_t)1;

    seq_t lcm = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t seq_i = (seq_t)i;
        seq_t g = gcd(lcm, seq_i);
        /* Avoid overflow in intermediate steps by dividing first. */
        lcm = (lcm / g) * seq_i;
    }
    return lcm - (seq_t)1;
}
