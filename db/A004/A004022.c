/*
 * A004022  Primes of the form (10^k - 1)/9. Also called repunit primes or
 *          repdigit primes.
 * https://oeis.org/A004022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, bref
 * Formula:   a(n) = A002275(A004023(n)) where A004023(n) gives the exponent k
 *            such that (10^k - 1)/9 is the n-th repunit prime
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"


static const uint64_t k_values[] = {2, 19, 23, 317, 1031};

seq_t a(idx_t n) {
    if (n < 1 || n > 5) return 0;
    
    uint64_t k = k_values[n - 1];
    seq_t pow10 = a000272_ipow((seq_t)10, k);
    return (pow10 - (seq_t)1) / (seq_t)9;
}
