/*
 * A088621  a(n) = smallest prime which is a concatenation of 1, n, n^2, n^3,
 *          ... n^k for some k, or 0 if no such prime exists.
 * https://oeis.org/A088621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the OEIS sequence A088621.
   For n ≥ 1, return the smallest prime that is a concatenation
   of 1, n, n², …, nᵏ for some k ≥ 1, or 0 if none exists. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static const idx_t MAX_ITER = 2000;      /* safe upper bound on k for n up to 10⁴ */

seq_t a(idx_t n)
{
    /* Even n or n divisible by 5 always give composite concatenations: */
    if ((n & 1ULL) == 0ULL || (n % 5ULL) == 0ULL)
        return (seq_t)0;

    seq_t cur   = (seq_t)1;      /* current concatenation (starts with '1')   */
    seq_t power = (seq_t)n;      /* nᵏ, initially k = 1                        */

    for (idx_t k = 1; k <= MAX_ITER; ++k)
    {
        /* Append nᵏ to cur */
        idx_t d = (idx_t)digit_len(power);        /* digits of nᵏ */
        seq_t shift = pow10(d);                   /* 10ᵈ          */
        cur = cur * shift + power;                /* cur = cur*10ᵈ + nᵏ */

        if (is_prime_big(cur))
            return cur;                          /* smallest prime found */

        /* Prepare for next power */
        power = power * (seq_t)n;
    }
    return (seq_t)0;                              /* no prime within limit */
}
