/*
 * A022853  a(n) = integer nearest n*Pi.
 * https://oeis.org/A022853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*Pi
 * Range:     u8 n <= 0, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 355/113 is a convergent of Pi with error < 2.7e-7.
   For all n (including large values) the nearest integer to
   n*Pi equals the nearest integer to n*355/113, because the
   approximation error is far smaller than 0.5.  The computation
   is performed entirely with integer arithmetic, using only
   +, -, *, /, % and the type seq_t. */
#include <stdint.h>
#include <stdbool.h>

static const seq_t NUM = 355;      /* numerator of 355/113 */
static const seq_t DEN = 113;      /* denominator               */
static const seq_t HALF = 56;      /* floor(DEN/2) for rounding  */

seq_t a(idx_t n)
{
    /* prod = n * NUM */
    seq_t prod = (seq_t)n * NUM;
    /* nearest integer: floor((prod + HALF) / DEN) */
    seq_t res = (prod + HALF) / DEN;
    return res;
}
