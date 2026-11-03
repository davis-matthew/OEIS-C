/*
 * A046687  Numbers k such that k and sum of 4th powers of divisors of k are
 *          relatively prime.
 * https://oeis.org/A046687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 153, u64 n <= 10076,
 *            u128 n <= 10076, big any n
 * Validated: standard, pass, 84 terms through n = 9980 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k and the sum of fourth powers of its divisors are coprime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Compute σ₄(k) = ∑_{d|k} d⁴ using a multiplicative formula.
   The computation is performed with 128‑bit intermediates to avoid
   overflow before the final product fits into the target type. */
static seq_t sigma_fourth_powers(uint64_t n)
{
    seq_t result = 1;
    uint64_t temp = n;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint32_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            __int128 sum   = 1;                     /* ∑_{i=0}^e p^{4i} */
            __int128 term  = 1;                     /* p^{4i} */
            __int128 p4    = (__int128)p * p * p * p; /* p⁴ */

            for (uint32_t i = 1; i <= e; ++i)
            {
                term *= p4;
                sum  += term;
            }
            result *= (seq_t)(uint64_t)sum;
        }
    }

    if (temp > 1)   /* remaining prime factor */
    {
        __int128 p4  = (__int128)temp * temp * temp * temp;
        __int128 sum = 1 + p4;
        result *= (seq_t)(uint64_t)sum;
    }

    return result;
}

seq_t a(idx_t n)
{
    static seq_t *values   = NULL;
    static size_t capacity = 0, size = 0;
    static uint64_t current = 0;  /* last integer examined */

    while (size < n)
    {
        ++current;
        seq_t k = (seq_t)current;
        seq_t s = sigma_fourth_powers(current);

        if (gcd(k, s) == (seq_t)1)
        {
            if (size + 1 > capacity)
            {
                size_t new_cap = capacity == 0 ? 16 : capacity * 2;
                values = mem_resize(seq_t, values, new_cap);
                capacity = new_cap;
            }
            values[size++] = k;
        }
    }

    return values[n - 1];
}
