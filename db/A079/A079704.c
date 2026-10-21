/*
 * A079704  a(n) = 2*prime(n)^2.
 * https://oeis.org/A079704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*prime(n)^2
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) corresponds to prime(1)=2 */
    if (n == 0) return 0;          /* undefined offset, safe return */
    uint32_t p = get_prime(n);     /* nth prime, 1‑based */
    seq_t val = (seq_t)p * (seq_t)p;
    val += val;                    /* multiply by 2 */
    return val;
}
