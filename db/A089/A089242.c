/*
 * A089242  Sequence is S(infinity), where S(1) = 1, S(m+1) = concatenation
 *          S(m), a(m)+1, S(m) and a(m) is the m-th term of S(m). a(m) is also
 *          the m-th term of the sequence.
 * https://oeis.org/A089242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 9369, u16 n <= 9369, u32 n <= 9369, u64 n <= 9369,
 *            u128 n <= 9369, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089242
 *  Formula:  a(1) = 1,
 *           a(n) = 1 + a(v2(n))   if n is even,
 *           a(n) = 1               if n is odd,
 *  where v2(n) is the exponent of 2 in the prime factorisation of n
 *  (the number of trailing zeros in the binary expansion of n).
 *
 *  This implements the recurrence a(n) = 1 + a(c(n)) where
 *  c(n) is the exponent of the largest power of two dividing n.
 *  The algorithm runs in O(log n) time and uses only integer arithmetic.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;           /* a(odd) = 1  */
    idx_t current = n;

    while ((current & 1ULL) == 0)   /* while n is even */
    {
        /* compute v2(current)  */
        idx_t exponent = 0;
        idx_t temp = current;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            exponent++;
        }

        current = exponent;       /* next argument of the recurrence */
        result++;                 /* each application adds 1 */
    }

    return result;
}
