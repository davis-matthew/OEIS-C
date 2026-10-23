/*
 * A101106  A Chebyshev transform of the central Delannoy numbers.
 * https://oeis.org/A101106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A101106 – Chebyshev transform of the central Delannoy numbers
 *
 * Recurrence:
 *   n*a(n) + 3*(-2*n+1)*a(n-1) + 3*(n-1)*a(n-2)
 *   + 3*(-2*n+3)*a(n-3) + (n-2)*a(n-4) = 0
 * with
 *   a(0) = 1,  a(1) = 3,  a(2) = 12,  a(3) = 57
 *
 * The function computes a(n) for any n ≥ 0 using the recurrence only
 * (no memoisation).  It therefore works for arbitrary call order and
 * stays well below the time limit for n up to about 10 000.
 */

#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    /* Initial values (offset 0) */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)57;

    /* a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)3;   /* a(1)   */
    seq_t a2 = (seq_t)12;  /* a(2)   */
    seq_t a3 = (seq_t)57;  /* a(3)   */
    seq_t ai;              /* current a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        /* term1 = 3*(2*i-1)*a(i-1) */
        seq_t term1 = a3;
        term1 *= (seq_t)(2*i-1);
        term1 *= (seq_t)3;

        /* term2 = 3*(i-1)*a(i-2) */
        seq_t term2 = a2;
        term2 *= (seq_t)(i-1);
        term2 *= (seq_t)3;

        /* term3 = 3*(2*i-3)*a(i-3) */
        seq_t term3 = a1;
        term3 *= (seq_t)(2*i-3);
        term3 *= (seq_t)3;

        /* term4 = (i-2)*a(i-4) */
        seq_t term4 = a0;
        term4 *= (seq_t)(i-2);

        /* numerator = term1 + term3 - term2 - term4 */
        seq_t numer = term1;
        numer += term3;
        numer -= term2;
        numer -= term4;

        /* a(i) = numer / i  (exact division) */
        ai = numer / (seq_t)i;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return ai;
}
