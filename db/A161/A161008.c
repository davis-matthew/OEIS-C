/*
 * A161008  Primes of the form 2*k^2 + 5939831.
 * https://oeis.org/A161008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 2*k^2 + 5939831
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 5939831, a(2) = 5939833, … */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    const seq_t C = (seq_t)5939831ULL;

    if (n == 0)   /* undefined for n < 1, but return 0 to avoid UB */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k = 0;
    seq_t candidate;

    while (count < n)
    {
        seq_t kk = (seq_t)k;
        candidate = kk * kk;               /* kk^2 */
        candidate = candidate * (seq_t)2;   /* 2*kk^2 */
        candidate = candidate + C;          /* 2*kk^2 + C */

        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++k;
    }

    /* Should never reach here for n >= 1 */
    return candidate;
}
