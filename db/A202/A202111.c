/*
 * A202111  a(n) = sigma(n) - p, where p is the largest prime < sigma(n).
 * https://oeis.org/A202111
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - p , where p is the largest prime < sigma(n)
 * Range:     u8 n <= 500, u16 n <= 8617, u32 n <= 8617, u64 n <= 8617,
 *            u128 n <= 8617, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t sigma = sigma_of(n);

    seq_t candidate = sigma - (seq_t)1;
    while (candidate > (seq_t)0) {
        if (is_prime_big(candidate))
            break;
        candidate -= (seq_t)1;
    }

    return sigma - candidate;
}
