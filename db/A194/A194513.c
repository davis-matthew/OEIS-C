/*
 * A194513  Second coordinate of (2,7)-Lagrange pair for n.
 * https://oeis.org/A194513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 2*floor((4*n + 6)/9)
 * Range:     i8 n <= 30, i16 n <= 97, i32 n <= 97, i64 n <= 97, i128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the nth term of OEIS sequence A194513.
   idx_t is an unsigned integer type and seq_t is a signed integer
   type large enough to hold the result. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t to avoid truncation   */
    seq_t temp = (seq_t)4 * nn + (seq_t)6;   /* 4*n + 6                            */
    seq_t q    = temp / (seq_t)9;            /* floor((4*n+6)/9)                    */
    return nn - (seq_t)2 * q;                /* n - 2*floor((4*n+6)/9)              */
}
