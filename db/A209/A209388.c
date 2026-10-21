/*
 * A209388  Product of positive odd integers smaller than n and relatively
 *          prime to n.
 * https://oeis.org/A209388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of odd k < n with gcd(k,n)=1
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 36, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k < n; k += 2) {
        if (gcd((seq_t)k, (seq_t)n) == (seq_t)1)
            prod *= (seq_t)k;
    }
    return prod;
}
