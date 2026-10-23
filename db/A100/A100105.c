/*
 * A100105  a(n) = 2^prime(n)-prime(n).
 * https://oeis.org/A100105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^prime(n) - prime(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t p = get_prime(n);
    seq_t pow = pow2((idx_t)p);
    return pow - p;
}
