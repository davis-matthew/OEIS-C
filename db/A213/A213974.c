/*
 * A213974  List of imprimitive words over the alphabet {2,3}.
 * https://oeis.org/A213974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213974 – list of imprimitive words over {2,3}
 *
 *  The terms are the imprimitive words over the alphabet {2,3},
 *  listed first by length (offset 1, length ≥ 2) and within each
 *  length in ascending decimal (lexicographic) order.
 *
 *  The routine enumerates all words of a given length in lexicographic
 *  order, tests each for the existence of a smaller period, counts
 *  the imprimitive ones, and returns the nth such word.
 *
 *  Complexity:  O(2^L * d(L)) where L is the smallest length that
 *  contains the nth word (d(L)=number of proper divisors of L).  For
 *  n ≤ 10⁴ this is trivial (L ≤ 15).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* We count imprimitive words as we generate them */
    idx_t count = 0;

    for (idx_t len = 2; ; ++len)        /* start at length 2 */
    {
        /* Number of candidates of this length = 2^len */
        /* len will never exceed about 20 for n≤10000 */
        unsigned long long total = 1ULL << len;
        if (total == 0)          /* overflow guard, unlikely for small n */
            return 0;

        for (unsigned long long i = 0; i < total; ++i)
        {
            /* Build digits and numeric value of the candidate */
            seq_t val = 0;
            /* Digit array for period check */
            static uint8_t digits[64];   /* enough for len<=64 */
            for (idx_t j = 0; j < len; ++j)
            {
                /* MSB first: bit at position (len-1-j) */
                uint8_t bit = (i >> (len - 1 - j)) & 1ULL;
                uint8_t digit = bit ? 3 : 2;          /* 2 ↔ 0, 3 ↔ 1 */
                digits[j] = digit;
                val = val * 10 + (seq_t)digit;
            }

            /* Test imprimitive: existence of a proper divisor d */
            bool imprimitive = false;
            for (idx_t d = 1; d < len; ++d)
            {
                if (len % d != 0)   /* only proper divisors matter */
                    continue;

                bool period = true;
                for (idx_t k = 0; k < len; ++k)
                {
                    if (digits[k] != digits[k % d])
                    {
                        period = false;
                        break;
                    }
                }
                if (period)
                {
                    imprimitive = true;
                    break;
                }
            }

            if (!imprimitive)
                continue;   /* skip primitive words */

            /* Found an imprimitive word */
            ++count;
            if (count == n)
                return val;
        }
    }

    /* Should never reach here for valid n */
    return 0;
}
