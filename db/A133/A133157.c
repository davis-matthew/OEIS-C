/*
 * A133157  Numbers k such that k^2 + k - 41 is prime.
 * https://oeis.org/A133157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k (offset 1) such that k^2 + k - 41 is
 *            prime
 * Range:     u8 none, u16 n <= 33, u32 n <= 41, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Any n == 0 is outside the offset; return 0. */
    if (n == 0) return (seq_t)0;

    idx_t found = 0;   /* how many suitable k's we have found */
    idx_t k = 0;       /* current integer being tested */

    while (1) {
        seq_t kk = (seq_t)k;                     /* cast to seq_t for arithmetic */
        seq_t val = kk * kk + kk - (seq_t)41;    /* k^2 + k - 41 */

        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return kk;                     /* return k when we hit the n-th. */
        }
        ++k;                                     /* try next k */
    }
}
