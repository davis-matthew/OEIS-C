/*
 * A211979  Numbers n formed by p 1's followed by p - 1 0's, where p is
 *          prime(n).
 * https://oeis.org/A211979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((10^p - 1) / 9) * 10^(p-1), * where p is the n-th prime
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    idx_t p = get_prime(n);          /* p = nth prime, p >= 2 for n >= 1 */
    seq_t ones = (pow10(p) - 1) / 9; /* value of n ones: (10^p - 1)/9 */
    seq_t result = ones * pow10(p - 1); /* append p-1 zeros: multiply by 10^(p-1) */
    return result;
}
