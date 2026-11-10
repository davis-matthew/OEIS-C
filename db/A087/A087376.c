/*
 * A087376  Leading diagonal of A087377.
 * https://oeis.org/A087376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest k such that n*k+1 is prime (last term of
 *            row n of A087377); computed by search
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 955 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (is_prime(n * k + 1)) ++count;
    }
    return (seq_t)k;
}
