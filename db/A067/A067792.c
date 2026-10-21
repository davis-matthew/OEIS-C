/*
 * A067792  a(n) is the least prime >= sigma(n).
 * https://oeis.org/A067792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least prime >= sigma(n)
 * Range:     u8 n <= 95, u16 n <= 9321, u32 n <= 9321, u64 n <= 9321,
 *            u128 n <= 9321, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);      /* σ(n) */
    seq_t p = s;
    while (!is_prime_big(p))    /* find the next prime ≥ σ(n) */
        ++p;
    return p;
}
