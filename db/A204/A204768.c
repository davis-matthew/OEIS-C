/*
 * A204768  7^p - 6^p - 1, with p = prime(n).
 * https://oeis.org/A204768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^p - 6^p - 1, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* p = n-th prime */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)p); /* compute 7^p */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)p); /* compute 6^p */
    return pow7 - pow6 - ((seq_t)1);      /* 7^p - 6^p - 1 */
}
