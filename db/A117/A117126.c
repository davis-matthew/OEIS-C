/*
 * A117126  Sum of all the different digits in the decimal expansion of 1/n.
 * https://oeis.org/A117126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of distinct digits in the decimal expansion of
 *            1/(n+1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t d = n + 1;                     /* denominator */
    if (d == 1) {                        /* 1/1 = 1.0 : digit 1 */
        return 1;
    }

    bool seen[10] = {false};
    seq_t sum = 0;

    /* track remainders to detect the repeating cycle of the decimal expansion */
    bool *visited = (bool *)malloc((size_t)d * sizeof(bool));
    if (!visited) {
        return 0;     /* allocation failed – return 0 as a safe fallback */
    }
    memset(visited, 0, (size_t)d * sizeof(bool));

    idx_t remainder = 1 % d;    /* start with remainder after dividing 1 by d */

    while (1) {
        if (remainder == 0) {          /* the decimal terminates */
            if (!seen[0]) {            /* include 0 if seen for the first time */
                seen[0] = true;
                sum += 0;             /* adding 0 changes nothing, kept for clarity */
            }
            break;
        }
        if (visited[remainder]) break;  /* cycle detected */
        visited[remainder] = true;

        idx_t tmp = remainder * 10;
        idx_t digit = tmp / d;
        remainder = tmp % d;

        if (!seen[digit]) {
            seen[digit] = true;
            sum += digit;
        }
    }

    free(visited);
    return sum;
}
