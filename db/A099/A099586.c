/*
 * A099586  Constant term in (1+x)^n mod (1+x^4).
 * https://oeis.org/A099586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 37, i64 n <= 72, i128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-4)  for n > 4
 *           a(1) = 1, a(2) = 1, a(3) = 1, a(4) = 0
 *
 * This implements the linear recurrence directly, which computes the
 * exact integer value for any n ≥ 1 in O(n) time.  All arithmetic is
 * performed in seq_t, so it works for 8,16,32,64,128‑bit integers or
 * arbitrary‑precision integers.
 */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 1 || n == 2 || n == 3) return (seq_t)1;
    if (n == 4)                     return (seq_t)0;

    /* maintain the four most recent terms a(n-4)...a(n-1) */
    seq_t a1 = 1;   /* a(1) */
    seq_t a2 = 1;   /* a(2) */
    seq_t a3 = 1;   /* a(3) */
    seq_t a4 = 0;   /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = 4 * a4 - 6 * a3 + 4 * a2 - 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return a4;
}
