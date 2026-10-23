/*
 * A135485  a(n) = Sum_{i=1..n} prime(i)^(i-1), where prime(i) denotes i-th
 *          prime number.
 * https://oeis.org/A135485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} prime(i)^(i-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);
        seq_t term = seq_pow(p, i - 1);
        sum += term;
    }
    return sum;
}
