/*
 * A066854  a(n) = Sum_{k=1..8} 2^(8-k) * c(16n+2k+1), where c(j) is 1 if j is
 *          composite, 0 if j is prime.
 * https://oeis.org/A066854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..8} 2^(8-k) * c(16n+2k+1), where c(j)=1 if j
 *            composite, 0 if prime
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (int k = 1; k <= 8; ++k) {
        idx_t val = 16 * n + (2 * k + 1);
        if (!is_prime(val))
            res |= ((seq_t)1 << (8 - k));
    }
    return res;
}
