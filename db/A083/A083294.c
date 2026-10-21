/*
 * A083294  a(n) = (4 + (-9)^n)/5.
 * https://oeis.org/A083294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (4 + (-9)^n) / 5 Recurrence: a(0) = 1, a(n) = 8 – 9 *
 *            a(n-1) for n ≥ 1
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case  */
    if (n == 0u)
        return (seq_t)1;

    /* Compute iteratively using the linear recurrence
       a(n) = 8 – 9 * a(n-1).  Multiplication by 9 is a small‑int
       operation on seq_t and works for any sign of a(n-1).  */
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1u; i <= n; ++i)
    {
        /* val = 8 – 9 * val  */
        seq_t tmp = val * ((seq_t)9);
        val = ((seq_t)8) - tmp;
    }
    return val;
}
