/*
 * A088816  Numbers of the form p^p - 2 where p is prime.
 * https://oeis.org/A088816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = get_prime(n)^get_prime(n) - 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first prime (2) */
    if (n == 0) {
        /* offset 1 means a(0) is unspecified; return 0 for safety */
        return 0;
    }
    uint32_t p = get_prime(n);
    seq_t v = big_pow((seq_t)p, (seq_t)p);
    return v - 2;
}
