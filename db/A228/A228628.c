/*
 * A228628  9's complement of prime(n).
 * https://oeis.org/A228628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^k - 1 - prime(n) where k = number of decimal digits of
 *            prime(n)
 * Range:     u8 n <= 25, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for n=0, return 0 */
    /* obtain the nth prime (1‑indexed) */
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;

    /* count digits of p */
    idx_t k = 0;
    uint64_t tmp = p;
    do {
        ++k;
        tmp /= 10;
    } while (tmp);

    /* compute 10^k */
    seq_t pow10k = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow10k *= (seq_t)10;

    /* return 10^k - 1 - prime(n) */
    return pow10k - (seq_t)1 - (seq_t)p;
}
