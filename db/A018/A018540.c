/*
 * A018540  Divisors of 598.
 * https://oeis.org/A018540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest divisor of 598 (598 = 2 * 13 * 23)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cached list of all divisors of 598 sorted increasingly. */
    static seq_t divisors[8];
    static bool initialized = false;

    if (!initialized)
    {
        /* Enumerate all 2^3 products of the distinct primes. */
        const seq_t primes[3] = {(seq_t)2, (seq_t)13, (seq_t)23};
        size_t i, j;
        size_t idx = 0;

        for (idx = 0; idx < 8; ++idx)
        {
            seq_t prod = 1;
            if (idx & 1) prod *= primes[0];
            if (idx & 2) prod *= primes[1];
            if (idx & 4) prod *= primes[2];
            divisors[idx] = prod;
        }

        /* Simple bubble‑sort to order the 8 small values. */
        for (i = 0; i < 8; ++i)
            for (j = i + 1; j < 8; ++j)
                if (divisors[j] < divisors[i])
                {
                    seq_t t = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = t;
                }

        initialized = true;
    }

    if (n >= 1 && n <= 8)
        return divisors[n - 1];
    else
        return 0;   /* Undefined for n > 8 (finite sequence). */
}
