/*
 * A038736  a(n) = T(3*n + 1, n + 1), array T as in A038792.
 * https://oeis.org/A038736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(4*n - j, j)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t term  = 1;               /* C(4*n, 0) */
    sum += term;

    idx_t four_n = 4 * n;

    for (idx_t j = 0; j < n; ++j)
    {
        /* recurrence: C(4n-(j+1), j+1) =
           C(4n-j, j) * (4n-2j)*(4n-2j-1) / ((4n-j)*(j+1)) */
        idx_t num1 = four_n - 2 * j;          /* 4n-2j            */
        idx_t num2 = num1 - 1;                /* 4n-2j-1          */
        idx_t den1 = four_n - j;              /* 4n-j             */
        idx_t den2 = j + 1;                   /* j+1              */

        /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds */
        __uint128_t tmp = (__uint128_t)term * num1;
        tmp *= num2;
        tmp /= den1;
        tmp /= den2;

        term = (seq_t)tmp;
        sum += term;
    }

    return sum;
}
