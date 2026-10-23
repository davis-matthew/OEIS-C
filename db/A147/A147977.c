/*
 * A147977  a(n) = 6^n-5^n-4^n-3^n-2^n-1.
 * https://oeis.org/A147977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^(n-1) - 5^(n-1) - 4^(n-1) - 3^(n-1) - 2^(n-1) - 1 for n
 *            ≥ 2, a(1) = -4
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A147977 is defined by the closed form above with the
   special value a(1) = -4; for any n ≥ 2 a(n) follows the simple power
   difference.  The implementation uses the big_pow helper from utils
   to avoid overflow for the arbitrary‑precision build. */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)(-4);          /* special first term */

    /* Exponent for n ≥ 2 is (n‑1) */
    seq_t exp = (seq_t)(n - 1);

    seq_t p6 = big_pow((seq_t)6, exp);
    seq_t p5 = big_pow((seq_t)5, exp);
    seq_t p4 = big_pow((seq_t)4, exp);
    seq_t p3 = big_pow((seq_t)3, exp);
    seq_t p2 = big_pow((seq_t)2, exp);

    return p6 - p5 - p4 - p3 - p2 - (seq_t)1;
}
