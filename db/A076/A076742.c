/*
 * A076742  Let u(1)=u(2)=1, u(n)=(2^floor(u(n-1)/2)+1)/u(n-2) then a(n) =
 *          numerator of u(n).
 * https://oeis.org/A076742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of u(n) where u(1)=u(2)=1, u(n) = (2^⌊u(n-1)/2⌋
 *            + 1) / u(n-2). The sequence is 36‑periodic
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer GCD for unsigned 64‑bit values. */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0u)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* The period length is a compile‑time constant. */
enum { PERIOD = 36 };

seq_t a(idx_t n)
{
    /* Periodic cache. */
    static seq_t period[PERIOD];
    static bool initialized = false;

    if (!initialized)
    {
        /* Current and previous terms of the recurrence, stored as
           numerator / denominator with all values in unsigned 64‑bit
           intermediates.  The real values always fit into seq_t. */
        uint64_t prev1_num = 1;   /* u(n-1) numerator  (start with u(2)) */
        uint64_t prev1_den = 1;   /* u(n-1) denominator */
        uint64_t prev2_num = 1;   /* u(n-2) numerator  (start with u(1)) */
        uint64_t prev2_den = 1;   /* u(n-2) denominator */

        /* Fill the period array.  period[0] and period[1] are the
           first two terms a(1) and a(2). */
        period[0] = 1;  /* a(1) */
        period[1] = 1;  /* a(2) */

        for (idx_t i = 3; i <= PERIOD; ++i)
        {
            /* Compute k = ⌊u(n-1)/2⌋.  All values are positive. */
            uint64_t k = prev1_num / (2u * prev1_den);

            /* X = 2^k + 1. */
            uint64_t X = ((uint64_t)1 << k) + 1u;

            /* Compute the new fraction u(n) = X / u(n-2).  That
               equals (X * prev2_den) / prev2_num. */
            uint64_t num = X * prev2_den;   /* numerator before reduction */
            uint64_t den = prev2_num;       /* denominator before reduction */

            /* Reduce by greatest common divisor. */
            uint64_t g = gcd64(num, den);
            num /= g;
            den /= g;

            /* Store the numerator as the next element of the period. */
            period[i - 1] = (seq_t)num;

            /* Update previous terms for the next iteration. */
            prev2_num = prev1_num;
            prev2_den = prev1_den;
            prev1_num = num;
            prev1_den = den;
        }

        initialized = true;
    }

    /* a(1) corresponds to period[0]; use modulo PERIOD for periodicity. */
    idx_t idx = (n - 1u) % PERIOD;
    return period[idx];
}
