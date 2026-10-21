/*
 * A217447  Number of n x n permutation matrices that disconnect their zeros.
 * https://oeis.org/A217447
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A217447
   Formula: a(n) = 4*∑_{i=1}^{n-2} i! - 2*(n-2)*∑_{i=0}^{n-4} i! + 2*∑_{i=1}^{n-3} i! + 2
   (offset: a(2) = 2)
   The function works for any seq_t (uint8_t…100-bit, arbitrary‑precision, etc.).
   For indices n < 2 the return value is 0. */

#include <stddef.h>  /* for NULL */

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }

    seq_t sum1 = (seq_t)0;   /* Σ_{i=1}^{n-2} i!        */
    seq_t sum3 = (seq_t)0;   /* Σ_{i=1}^{n-3} i!        */
    seq_t sum2 = (seq_t)0;   /* Σ_{i=0}^{n-4} i!        */

    /* compute factorials up to (n-2)! and accumulate sum1 and sum3 */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n - (idx_t)2; ++i) {
        fact *= (seq_t)i;
        sum1 += fact;
        if (i <= n - (idx_t)3) {
            sum3 += fact;
        }
    }

    /* compute sum2 for i = 0 .. n-4  (only if n >= 4) */
    if (n >= (idx_t)4) {
        seq_t fact2 = (seq_t)1;  /* 0! = 1 */
        sum2 += fact2;           /* add 0! */
        for (idx_t i = (idx_t)1; i <= n - (idx_t)4; ++i) {
            fact2 *= i;
            sum2 += fact2;
        }
    }

    /* a(n) = (4*sum1 + 2*sum3 + 2) - 2*(n-2)*sum2   (termA >= termB) */
    seq_t termA = (seq_t)4 * sum1 + (seq_t)2 * sum3 + (seq_t)2;
    seq_t termB = (seq_t)2 * (n - (idx_t)2) * sum2;

    return termA - termB;
}
