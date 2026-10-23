/*
 * A173013  a(1) = 1, for n >= 2; a(n) = the smallest number h such that
 *          sigma(h) = A000203(h) = a(n-1) + 3, a(n) = 0 if no such number
 *          exists.
 * https://oeis.org/A173013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3, a(3)=5, a(4)=7; for n>=5,
   a(n)=0 if n is odd, a(n)=2 if n is even. */
#include <stdint.h>

/* Computes OEIS sequence A173013. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)7;
    return (n & 1) ? (seq_t)0 : (seq_t)2;
}
