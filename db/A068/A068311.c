/*
 * A068311  Arithmetic derivative of n!.
 * https://oeis.org/A068311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of n! (OEIS A068311)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 430 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Compute the arithmetic derivative of an integer k using trial‑division. */
static seq_t derivative_of(uint64_t k)
{
    seq_t dk = 0;
    uint64_t temp = k;

    /* factorise k */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            dk += ((seq_t)k / (seq_t)p) * (seq_t)e;
        }
    }

    /* remaining prime factor */
    if (temp > 1)
    {
        dk += ((seq_t)k / (seq_t)temp) * (seq_t)1;
    }

    return dk;
}

/*  The main sequence function required by the problem. */
seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0 since 0! = 1! = 1 and d(1) = 0 */
    if (n <= 1)
        return 0;

    seq_t fact   = 1;   /* (k‑1)!   */
    seq_t deriv  = 0;   /* d((k‑1)!) */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t dk = derivative_of((uint64_t)k);

        /* Update factorial and its derivative:
           d(k!) = d((k‑1)!)*k + (k‑1)!*d(k)             */
        deriv = deriv * (seq_t)k + fact * dk;
        fact *= (seq_t)k;
    }

    return deriv;
}
