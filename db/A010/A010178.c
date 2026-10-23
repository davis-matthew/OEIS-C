/*
 * A010178  Continued fraction for sqrt(113).
 * https://oeis.org/A010178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 10, a(1..8) = 1,1,1,2,2,1,1,1, a(9) = 20, and a(n) =
 *            a(n-9) for n >= 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The repeating block of the continued fraction (excluding the initial
   a0). */
static const seq_t period[9] = {1, 1, 1, 2, 2, 1, 1, 1, 20};

/* Return the nth term of A010178 (continued fraction for sqrt(113)). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 10;                  /* a0 */
    }
    /* For n >= 1 the pattern repeats every 9 terms.  The index into
       'period' is (n-1) mod 9. */
    return period[(n - 1) % 9];
}
