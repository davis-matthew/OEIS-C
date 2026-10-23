/*
 * A043707  Numbers whose base-4 representation has an odd number of runs.
 * https://oeis.org/A043707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose base‑4 representation
 *            has an odd number of runs
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for zero index, though not used */
    idx_t count = 0;
    idx_t i = 1;
    seq_t result = 0;

    while (count < n) {
        idx_t x = i;
        int transitions = 0;
        int prev = -1;
        while (x > 0) {
            int d = x % 4;
            if (prev != -1 && d != prev) transitions++;
            prev = d;
            x /= 4;
        }
        /* odd number of runs ⇔ even number of transitions */
        if ((transitions & 1) == 0) {
            count++;
            if (count == n) {
                result = (seq_t)i;
                break;
            }
        }
        i++;
    }
    return result;
}
