/*
 * A197943  Greatest residue of x^(n-1) (mod n), x=0..n-1.
 * https://oeis.org/A197943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 11111, u32 n <= 11111, u64 n <= 11111,
 *            u128 n <= 11111, big any n
 * Validated: standard, pass, 107 terms through n = 8102 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

/* Computes the OEIS sequence A197943:
   a(n) = max_{0 <= x < n} (x^(n-1) mod n)   (n >= 1)
   Special cases are handled explicitly: prime n -> 1,
   even n -> n-1. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    if (is_prime(n))
        return 1;

    if ((n & 1) == 0)
        return (seq_t)n - 1;

    seq_t max = 0;
    idx_t exp = n - 1;
    seq_t mod = (seq_t)n;

    for (idx_t x = 0; x < n; ++x) {
        seq_t r = powmod((seq_t)x, exp, mod);
        if (r > max)
            max = r;
    }
    return max;
}
