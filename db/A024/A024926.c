/*
 * A024926  a(n) = Sum_{k=1..n} floor(prime(k)/k).
 * https://oeis.org/A024926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(prime(k)/k)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk = (seq_t)get_prime(k);
        sum += pk / k;
    }
    return sum;
}
