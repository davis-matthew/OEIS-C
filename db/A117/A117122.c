/*
 * A117122  Number of different digits in the decimal expansion of 1/n.
 * https://oeis.org/A117122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct decimal digits in the expansion of 1/n */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;           /* 1/1 = 1 → only digit 1 */

    bool digit_seen[10] = { false };
    digit_seen[0] = true;            /* the integer part “0” is present for n>1 */

    /* remember which remainders we have already seen */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;            /* graceful fallback on allocation failure */

    idx_t rem = 1 % n;               /* starting remainder for 1/n */

    while (1) {
        if (rem == 0) break;         /* terminating expansion */
        if (seen[(size_t)rem]) break;/* found cycle of the repeating part */
        seen[(size_t)rem] = true;

        __int128 prod = (__int128)rem * 10;
        unsigned int digit = (unsigned int)(prod / n);
        digit_seen[digit] = true;

        rem = (idx_t)(prod % n);
    }

    free(seen);

    seq_t res = 0;
    for (int d = 0; d < 10; ++d)
        if (digit_seen[d]) res = res + 1;

    return res;
}
