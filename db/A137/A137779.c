/*
 * A137779  Number of bases (numbering systems, including unary) in which the
 *          n-th prime is a palindrome having at least two digits.
 * https://oeis.org/A137779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = A126071(prime(n))
 * Range:     u8 n <= 54, u16 n <= 6577, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of bases (including unary) in which the n‑th prime is
   a palindrome having at least two digits. */

/* Required utilities are supplied by the build system. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* the offset of the sequence is 1, so n==0 is an unused case */
    if (n == 0) return 0;

    /* Retrieve the n‑th prime.  get_prime returns a 32‑bit value. */
    uint32_t p32 = get_prime(n);
    idx_t    p   = (idx_t)p32;          /* for loop calculations */

    /* base 1 (unary) always yields a palindrome with at least two digits */
    seq_t count = 1;

    /* base p-1 gives the two‑digit palindrome “11”; count it only when
       p > 2 to avoid double counting the unconditional base 1 for p=2. */
    if (p32 > 2) ++count;

    /* Any palindrome of length ≥ 3 must have base ≤ √p.  Therefore
       we test only bases 2 … floor(√p).  The loop condition b*b ≤ p
       guarantees this. */
    for (idx_t b = 2; b * b <= p; ++b)
    {
        if (is_palindrome((seq_t)p32, b))
            ++count;
    }

    return count;
}
