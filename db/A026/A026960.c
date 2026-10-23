/*
 * A026960  a(n) = Sum_{k=0..n} (k+1) * A026615(n,k).
 * https://oeis.org/A026960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n+2)*2^(n-3) - n - 2 for n > 1 a(0) = 1, a(1) = 3,
 *            a(2) = 10
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the helper pow2(idx_t) to compute 2^(n-3). */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 10;

    /* For n >= 3 the formula applies directly. */
    seq_t val = pow2(n - 3);          /* 2^(n-3) */
    seq_t mult = 7 * (seq_t)(n + 2);  /* 7*(n+2) */
    val *= mult;                     /* 7*(n+2)*2^(n-3) */
    val -= n;                        /* subtract n */
    val -= 2;                        /* subtract 2 */
    return val;
}
