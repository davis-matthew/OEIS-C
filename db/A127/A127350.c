/*
 * A127350  a(n) = Sum_{i=n..n+3} Sum_{j=i+1..n+4} prime(i)*prime(j).
 * https://oeis.org/A127350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=n}^{n+3} Sum_{j=i+1}^{n+4} prime(i)*prime(j) (i.e.
 *            sum of products of all unordered pairs of the five consecutive
 *            primes starting at index n.)
 * Range:     u8 none, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* undefined for n < 1, returning 0 */
    }

    uint32_t p[5];
    for (int k = 0; k < 5; ++k) {
        p[k] = get_prime(n + k);
    }

    seq_t sum = 0;
    for (int i = 0; i < 5; ++i) {
        seq_t pi = (seq_t)p[i];
        for (int j = i + 1; j < 5; ++j) {
            sum += pi * (seq_t)p[j];
        }
    }

    return sum;
}
