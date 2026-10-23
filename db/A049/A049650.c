/*
 * A049650  Compositorial numbers (A036691) + 1.
 * https://oeis.org/A049650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Product_{i=1..n} composite(i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    idx_t count = 0;
    idx_t value = 4;
    seq_t prod = (seq_t)1;
    while (count < n) {
        if (!is_prime(value)) {
            prod = prod * (seq_t)value;
            ++count;
        }
        ++value;
    }
    return prod + (seq_t)1;
}
