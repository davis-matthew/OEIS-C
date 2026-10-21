/*
 * A215761  Numbers m with property that 36m+11 is prime.
 * https://oeis.org/A215761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215761: numbers m such that 36*m + 11 is prime. */
/* Computes the n‑th such m (offset 1). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n == 0 is undefined for this offset; return 0 as a safe guard */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t m = 0;

    while (true)
    {
        idx_t p = 36 * m + 11;
        if (is_prime(p))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }

    /* unreachable */
    return (seq_t)0;
}
