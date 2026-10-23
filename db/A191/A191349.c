/*
 * A191349  Binomial sums: a(n) = Sum_{k=0..floor(n/3)} binomial(n-k,2*k)^2.
 * https://oeis.org/A191349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-k,2*k)^2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t b = binomial(n - k, 2 * k);
        sum += b * b;
    }
    return sum;
}
