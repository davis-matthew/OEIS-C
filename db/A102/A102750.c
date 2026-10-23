/*
 * A102750  Numbers n such that square of largest prime dividing n does not
 *          divide n.
 * https://oeis.org/A102750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 8691, u32 n <= 8691, u64 n <= 8691,
 *            u128 n <= 8691, big any n
 * Helpers:   good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102750: Numbers n such that the largest prime factor of n has exponent 1
   (i.e. the square of the largest prime dividing n does not divide n).
   The function iterates through integers, tests the condition by factorisation
   with trial division, and returns the n-th such number. */

#include <stdbool.h>
#include "../utils/good.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    seq_t value = 1;                /* start before first candidate */
    idx_t count = 0;

    while (count < n)
    {
        ++value;                    /* candidate integer */
        if (good(value))
            ++count;
    }

    return value;                   /* n-th term */
}
