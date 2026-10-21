/*
 * A061006  a(n) = (n-1)! mod n.
 * https://oeis.org/A061006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! mod n. Computation: for n ≤ 1 → 0; n = 4 → 2; if n
 *            is prime → n-1; otherwise 0
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    if (n == 4)
        return 2;
    if (is_prime(n))
        return (seq_t)n - 1;
    return 0;
}
