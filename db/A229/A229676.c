/*
 * A229676  a(n) = Sum_{k = 0..n} Product_{j = 0..8} C(n+j*k,k).
 * https://oeis.org/A229676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} Product_{j=0..8} C(n + j*k, k)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t prod = 1;
        for (int j = 0; j <= 8; ++j) {
            prod *= binomial(n + (idx_t)j * k, k);
        }
        total += prod;
    }
    return total;
}
