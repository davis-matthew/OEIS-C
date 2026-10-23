/*
 * A174530  Numerators of the second row of the Akiyama-Tanigawa table for the
 *          sequence 1/n!.
 * https://oeis.org/A174530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A174530: Numerators of the second row of the Akiyama–Tanigawa table for 1/n!.
   For n ≥ 2: a(n) = n+1 if n+1 is prime; otherwise a(n) = 1.
   Special cases: a(0) = -1, a(1) = 0, a(3) = 4 (because 4 = n+1). */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)(-1);      /* a(0) = -1 */
    if (n == 1) return 0;               /* a(1) = 0 */
    if (n == 3) return 4;               /* a(3) = 4 (special composite case) */

    /* For n ≥ 2, n+1 fits in idx_t (uint64_t). */
    idx_t val = n + 1;
    if (is_prime(val))
        return (seq_t)val;              /* prime → numerator = n+1 */
    else
        return 1;                       /* composite → numerator = 1 */
}
