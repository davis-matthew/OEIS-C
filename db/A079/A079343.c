/*
 * A079343  Period 6: repeat [0, 1, 1, 2, 3, 1]; also F(n) mod 4, where F(n) =
 *          A000045(n).
 * https://oeis.org/A079343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Fibonacci(n) modulo 4. * The sequence is periodic with
 *            period 6: [0,1,1,2,3,1]. * Therefore a(n) = a(n mod 6)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

/* Return the n‑th term of A079343.  idx_t is an unsigned integer type
 * (e.g., uint64_t).  seq_t is an unsigned integer type of the build */
seq_t a(idx_t n)
{
    /* Period 6 for Fibonacci modulo 4 */
    static const seq_t period[6] = {0, 1, 1, 2, 3, 1};

    /* n is idx_t, % 6 gives an idx_t < 6; cast to size_t for array indexing */
    return period[(size_t)(n % 6)];
}
