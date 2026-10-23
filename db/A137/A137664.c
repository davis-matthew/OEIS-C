/*
 * A137664  a(n) = (p+1)^p - 1 where p = prime(n).
 * https://oeis.org/A137664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)+1)^prime(n)-1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime number */
    uint32_t p = get_prime(n);

    /* Compute (p+1)^p */
    seq_t base = (seq_t)p + 1;
    seq_t result = seq_pow(base, (idx_t)p);

    /* Subtract 1 to obtain a(n) */
    return result - (seq_t)1;
}
