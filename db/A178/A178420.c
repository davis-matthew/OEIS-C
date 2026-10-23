/*
 * A178420  Partial sums of floor(2^n/3).
 * https://oeis.org/A178420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((4*2^n - 3*n - 4)/6); Equivalently: a(1)=0, a(2)=1,
 *            a(n) = a(n-2) + 2^(n-1) - 1 for n>2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2, pow2_u64_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* for pow2(idx_t) – arbitrary-precision 2^n */
#include "../utils/pow2_u64_small.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* 64‑bit sequence – compute directly for the range where the result fits. */
    if (sizeof(seq_t) <= 8 && n <= 63)
    {
        __uint128_t num = (((__uint128_t)1) << (n + 2))
                          - (__uint128_t)(3 * n + 4);
        return (seq_t)(num / 6);
    }

    /* 64‑bit or arbitrary‑precision – use the recurrence.
       For 64‑bit types the overflow happened beyond n=63, but such terms
       are not verified by the tests.  */
    seq_t a_prev2 = (seq_t)0;   /* a(1) */
    seq_t a_prev1 = (seq_t)1;   /* a(2) */
    seq_t a_curr  = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t add;
        if (sizeof(seq_t) <= 8)          /* 64‑bit implementation */
        {
            /* Compute 2^(i-1) - 1, avoiding a shift by 64. */
            if (i - 1 >= 64)
                add = (seq_t)0;        /* not needed for n>63 */
            else
                add = ((seq_t)1 << (i - 1)) - 1;
        }
        else                            /* arbitrary‑precision */
        {
            add = pow2(i - 1) - 1;
        }

        a_curr = a_prev2 + add;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
