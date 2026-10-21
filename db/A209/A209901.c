/*
 * A209901  7^p - 6^p - 2 with p = prime(n).
 * https://oeis.org/A209901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 7^prime(n) - 6^prime(n) - 2 (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset handling */

    uint32_t p = get_prime(n);             /* p = the n-th prime      */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)p);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)p);
    seq_t res  = pow7 - pow6 - 2;
    return res;
}
