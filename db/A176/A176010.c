/*
 * A176010  Positive numbers k such that k^2 == 2 (mod 97).
 * https://oeis.org/A176010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-97 + 41*(-1)^n + 194*n)/4
 * Range:     u8 n <= 1, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(1)=14; if n even a(n)=a(n-1)+69 else a(n)=a(n-1)+28 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is valid for n >= 1.  For robustness we handle n==0 by
       returning 0, though it is outside the OEIS offset. */
    if (n == 0)
        return 0;

    /* Compute using unsigned arithmetic.  The intermediate value
       194*n is always non‑negative and large enough to contain the
       final result for all practical n. */
    seq_t val = (seq_t)n * (seq_t)194;

    if ((n & 1) == 0)          /* n even  */
        val += 41;
    else                       /* n odd   */
        val -= 41;

    val -= 97;
    val /= 4;

    return val;
}
