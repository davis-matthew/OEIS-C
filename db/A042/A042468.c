/*
 * A042468  Numerators of continued fraction convergents to sqrt(762).
 * https://oeis.org/A042468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of the convergents to √762.
   a(0) = 27,
   a(1) = 28,
   for n >= 2:
      let b[n-1] = {1,1,1,1,8,1,1,1,1,54}[ (n-1) % 10 ]
      a(n) = b[n-1] * a(n-1) + a(n-2)   */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const int periodic[10] = {1,1,1,1,8,1,1,1,1,54};

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) {
        return (seq_t)27;
    }
    if (n == 1) {
        return (seq_t)28;
    }

    /* Work backwards using the recurrence */
    seq_t p_im2 = (seq_t)27;   /* a(0) */
    seq_t p_im1 = (seq_t)28;   /* a(1) */
    seq_t p_n    = 0;

    for (idx_t i = 2; i <= n; ++i) {
        int coef = periodic[(i - 1) % 10];
        p_n = (seq_t)coef * p_im1 + p_im2;
        p_im2 = p_im1;
        p_im1 = p_n;
    }

    return p_n;
}
