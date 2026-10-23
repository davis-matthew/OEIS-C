/*
 * A165113  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-9 Kaprekar map in A165110.
 * https://oeis.org/A165113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct terms in the trajectory of n under the
 *            * base‑9 Kaprekar map until the first repeated value. Each value
 *            * counted once: pre‑period length + cycle length
 * Range:     u8 n <= 82, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   kaprekar9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/kaprekar9.h"

/* Maximum number of distinct terms we expect to see in any trajectory
   for the ranges considered (far larger than needed). */
#define MAX_TRAJECTORY 2048

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* Store all distinct values encountered so far. */
    seq_t seen[MAX_TRAJECTORY];
    size_t m = 0;                /* number of distinct terms seen */

    seen[m++] = x;

    while (1) {
        seq_t next = kaprekar9(x);

        /* Does 'next' already appear in 'seen'? */
        for (size_t i = 0; i < m; ++i) {
            if (seen[i] == next) {
                return (seq_t)m;
            }
        }

        seen[m++] = next;
        x = next;
    }
}
