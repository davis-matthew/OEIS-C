/*
 * A035091  Smallest prime == 1 mod (n^2).
 * https://oeis.org/A035091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime of the form n^2*k + 1 with k ≥ 1
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 378, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is computed by a simple search of the arithmetic
    progression n^2·k + 1 until a prime is found, using the arbitrary‑
    precision primality test provided by the build. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The offset for the sequence is 1, so a(1) is the first
       term.  For safety, return 0 for n=0, though it is never used. */
    if (n == 0) return 0;

    /* base = n^2, computed in the arbitrary precision type seq_t */
    seq_t base = (seq_t)n * (seq_t)n;

    /* Search for the first k ≥ 1 such that base * k + 1 is prime. */
    for (idx_t k = 1; ; ++k)
    {
        seq_t candidate = base * (seq_t)k + 1;
        if (is_prime_big(candidate))
            return candidate;
    }
}
