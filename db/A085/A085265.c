/*
 * A085265  Numbers that can be written as sum of a positive squarefree number
 *          and a positive square.
 * https://oeis.org/A085265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_squarefree64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085265 – numbers that can be written as sum of a positive squarefree
 *  integer and a positive square.
 *
 *  a(n) is the n‑th integer (>1) that can be expressed as
 *  a + b² where a ≥ 1 is squarefree and b ≥ 1.  The sequence begins
 *  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, …
 *
 *  The algorithm simply tests successive integers and counts those that
 *  satisfy the condition.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree64.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1 – return 0 for n==0 */
        return (seq_t)0;

    unsigned long long count = 0;   /* how many qualifying numbers found */
    unsigned long long cand  = 2;   /* current integer to test */

    /* Keep searching until we have counted n qualifying numbers. */
    while (count < (unsigned long long)n) {
        /* For the current candidate, test all squares b² < cand. */
        unsigned long long limit = cand - 1;
        unsigned long long lastPossibleB = 0;

        /* Determine the largest b such that b² ≤ limit. */
        for (unsigned long long b = 1; b * b <= limit; ++b)
            lastPossibleB = b;

        bool found = false;
        for (unsigned long long b = 1; b <= lastPossibleB; ++b) {
            unsigned long long a_part = cand - b * b;
            if (a_part >= 1 && is_squarefree64(a_part)) {
                ++count;
                found = true;
                break;
            }
        }

        if (!found)
            /* candidate does not satisfy the requirement – do nothing */
            ;
        ++cand;
    }

    /* cand was incremented one step past the last counted number */
    return (seq_t)(cand - 1);
}
