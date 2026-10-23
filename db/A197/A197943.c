/*
 * A197943  Greatest residue of x^(n-1) (mod n), x=0..n-1.
 * https://oeis.org/A197943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 8102, u32 n <= 8102, u64 n <= 8102,
 *            u128 n <= 8102, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

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
        seq_t r = mod_pow((seq_t)x, exp, mod);
        if (r > max)
            max = r;
    }
    return max;
}
