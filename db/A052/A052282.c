/*
 * A052282  Number of 3 X 3 stochastic matrices under row and column
 *          permutations.
 * https://oeis.org/A052282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The sequence A052282 has the rational generating function (x^6 -
 *            x^5 + x^3 - x + 1) / ((1-x)^5 (1+x)^2 (1+x+x^2)) which implies a
 *            linear recurrence of order 9 a(n) = 2 a(n-1) + a(n-2) - 3 a(n-3)
 *            - a(n-4) + a(n-5) + 3 a(n-6) - a(n-7) - 2 a(n-8) + a(n-9) + c(n)
 *            where the correction term is c(0)= 1,...
 * Range:     u8 n <= 14, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ring buffer of the last 10 values, indices modulo 10 */
    seq_t buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t pos = 0, neg = 0;

        if (i >= 1) pos += 2 * buf[(i - 1) % 10];
        if (i >= 2) pos += 1 * buf[(i - 2) % 10];
        if (i >= 3) neg += 3 * buf[(i - 3) % 10];
        if (i >= 4) neg += 1 * buf[(i - 4) % 10];
        if (i >= 5) pos += 1 * buf[(i - 5) % 10];
        if (i >= 6) pos += 3 * buf[(i - 6) % 10];
        if (i >= 7) neg += 1 * buf[(i - 7) % 10];
        if (i >= 8) neg += 2 * buf[(i - 8) % 10];
        if (i >= 9) pos += 1 * buf[(i - 9) % 10];

        seq_t val = pos - neg;         /* main recurrence part */

        /* add the correction term c(n) */
        switch (i)
        {
            case 0: val += 1; break;
            case 1: val -= 1; break;
            case 3: val += 1; break;
            case 5: val -= 1; break;
            case 6: val += 1; break;
            default: /* c(n)=0 */ break;
        }

        buf[i % 10] = val;
    }
    return buf[n % 10];
}
