/*
 * A164383  Composite numbers of the form 4 + some prime.
 * https://oeis.org/A164383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 + p, where p is the nth prime such that 4+p is
 *            composite
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1 */

    idx_t count = 0;
    idx_t p = 2;

    while (count < n)
    {
        /* Test if p is prime */
        if (is_prime_big((seq_t)p))
        {
            seq_t val = (seq_t)p + 4;
            /* Accept p only if 4+p is composite */
            if (!is_prime_big(val))
            {
                ++count;
                if (count == n)
                    return val;
            }
        }
        ++p;
    }

    /* Should never reach here for valid n */
    return (seq_t)0;
}
