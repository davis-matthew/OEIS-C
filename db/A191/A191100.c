/*
 * A191100  [Squarefree part of (ABC)]/C for A=3, C=A+B, as a function of B,
 *          rounded to nearest integer.
 * https://oeis.org/A191100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round( SQP(3*n*(n+3)) / (n+3) ), where SQP(x) is the
 *            square‑free part of x (product of its distinct prime factors)
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – behaviour irrelevant */

    seq_t A = (seq_t)3;              /* fixed A */
    seq_t B = (seq_t)n;              /* B = n */
    seq_t C = A + B;                 /* C = A + B = n + 3 */
    seq_t m = A * B * C;             /* product ABC = 3*n*(n+3) */

    /* compute the square‑free part rad(m) = product of distinct prime factors of m */
    seq_t rad = (seq_t)1;
    seq_t temp = m;
    seq_t limit = floor_sqrt(m);

    for (seq_t p = (seq_t)2; p <= limit; ++p)
    {
        if (temp % p == 0)
        {
            rad *= p;                /* include this prime once */
            while (temp % p == 0)
                temp /= p;           /* remove all powers of p */
            limit = floor_sqrt(temp);  /* shrink the search bound */
        }
    }
    if (temp > (seq_t)1)  /* remaining prime factor */
        rad *= temp;

    /* round rad / C to the nearest integer (half rounds up) */
    seq_t den  = C;
    seq_t num  = rad;
    seq_t half = den / (seq_t)2;
    seq_t rounded = (num + half) / den;

    return rounded;
}
