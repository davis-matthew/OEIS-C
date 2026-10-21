/*
 * A238847  Smallest k such that k*n^3 + 1 is prime.
 * https://oeis.org/A238847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k*n^3 + 1 is prime
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 16, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* n ≥ 1; cast to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t n3 = nseq * nseq * nseq;
    seq_t k = 1;
    while (1) {
        seq_t p = k * n3 + 1;
        if (is_prime_big(p))
            return k;
        ++k;
    }
}
