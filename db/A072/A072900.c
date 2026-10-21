/*
 * A072900  Discriminant of quadratic field Q(sqrt(prime(n))) where prime(n)
 *          is the n-th prime.
 * https://oeis.org/A072900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p % 4 == 1) ? p : 4*p, where p is the n‑th prime
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    uint64_t p64 = get_prime((idx_t)n);   /* helper returns the n‑th prime */
    seq_t p = (seq_t)p64;

    seq_t mod4 = p % 4;
    if (mod4 == 1)
        return p;
    return p * 4;
}
