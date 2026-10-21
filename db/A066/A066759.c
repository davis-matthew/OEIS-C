/*
 * A066759  a(n) = least multiple of n differing from a prime by at most 1, if
 *          such a multiple exists; = 0 otherwise.
 * https://oeis.org/A066759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least multiple of n that differs from a prime by at most
 *            1; a(n) = 0 if no such multiple exists. The function searches
 *            for the smallest positive integer k such that n·k is within 1 of
 *            a prime
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We rely on the utility function
        bool is_prime_big(seq_t n)
   which is available in the build.  It performs a deterministic
   primality test for values that fit in 64 bits and a probable test
   for larger values (the sequence data for n ≤ 10⁴ never exceeds 64
   bits, so the result is exact).  For small seq_t types the test
   automatically works on the wrapped values that fit.  The algorithm
   is O(k) with a very small k in practice. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    if (n == 0)          /* index 0 is outside the offset of the sequence */
        return (seq_t)0;

    seq_t sn = (seq_t)n; /* convert index to seq_t so that arithmetic
                            is performed in the correct type */
    for (idx_t k = 1; ; ++k)
    {
        /* compute m = n * k   (wraps modulo if seq_t is too small) */
        seq_t m = sn * (seq_t)k;

        /* if we hit an overflow, the product will wrap to 0; stop searching */
        if (m == (seq_t)0)
            break;

        /* check if m, m-1, or m+1 is prime */
        if (is_prime_big(m) || (m > (seq_t)1 && is_prime_big(m - (seq_t)1))
                        || is_prime_big(m + (seq_t)1))
            return m;
    }

    /* no suitable multiple was found (only possible if an overflow
       hid the answer).  In the range of interest this never occurs. */
    return (seq_t)0;
}
