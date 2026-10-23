/*
 * A137447  a(n) = 4*a(n-4) for n>3, a(0)=-1, a(1)=-4, a(2)=2, a(3)=12.
 * https://oeis.org/A137447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-4) for n>3; a(0) = -1, a(1) = -4, a(2) = 2, a(3) =
 *            12
 * Range:     i8 n <= 10, i16 n <= 26, i32 n <= 58, i64 n <= 122,
 *            i128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init[4] = { (seq_t)-1, (seq_t)-4, (seq_t)2, (seq_t)12 };

/* Computes a(n) from OEIS sequence A137447.
   Uses the recurrence a(n)=4*a(n-4) which implies
   a(4k+r)=4^k * a(r) for r in {0,1,2,3}.  */
seq_t a(idx_t n)
{
    if (n < 4) return init[n];

    idx_t k = n / 4;         /* power of four to apply */
    idx_t r = n % 4;         /* remainder index for base value */

    seq_t result = init[r];
    /* multiply by 4^k by repeated multiplication (k ≤ n/4 ≤ 2500 for n≈10000) */
    for (idx_t i = 0; i < k; ++i)
        result *= (seq_t)4;

    return result;
}
