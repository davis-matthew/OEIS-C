/*
 * A128698  Alternating sum of the eighth powers of the first n Fibonacci
 *          numbers.
 * https://oeis.org/A128698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^k * F(k)^8 where F(k) is the k‑th
 *            Fibonacci number
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the multiplication helper defined in the utils library. */
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* sum_{k=1..n} (-1)^k F(k)^8 */
    seq_t sum = 0;                     /* running total */
    seq_t f_prev = 0;                  /* F(k-1) */
    seq_t f = 1;                       /* F(k)  */
    int sign = -1;                     /* (-1)^k at start of loop */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute F(k)^8 via repeated squaring: (f^2)^4 */
        seq_t f2 = seq_mul(f, f);      /* f^2 */
        seq_t f4 = seq_mul(f2, f2);    /* f^4 */
        seq_t f8 = seq_mul(f4, f4);    /* f^8 */

        /* Add the signed term to the total */
        sum = sum + (seq_t)sign * f8;
        sign = -sign;                  /* toggle sign for next k */

        /* Advance Fibonacci: F(k+1) = F(k) + F(k-1) */
        seq_t next = f_prev + f;       /* using builtin + which works for
                                         * all supported seq_t types */
        f_prev = f;
        f = next;
    }

    return sum;
}
