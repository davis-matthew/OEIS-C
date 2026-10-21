/*
 * A086829  Number of n X n matrices on n^2 distinct symbols modulo rotations
 *          and reflections.
 * https://oeis.org/A086829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2)!/8 for n>1, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the factorial of n^2 using arbitrary precision arithmetic
   provided by the build system and divides by 8.  This implementation
   is straightforward and works for all n for which the value fits in
   the representation of seq_t.  The factorial is accumulated iteratively
   from 2 to n^2.  For small to moderate n this is efficient enough
   for the expected test suite.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1) return (seq_t)1;

    /* compute n^2 safely in seq_t to avoid idx_t overflow */
    seq_t sq = (seq_t)n * (seq_t)n;

    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= sq; ++i)
    {
        result *= (seq_t)i;
    }

    /* (n^2)! is divisible by 8 for any n > 1 */
    result /= (seq_t)8;
    return result;
}
