/*
 * A043898  Numbers k such that 0 and 3 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that 0 and 3 occur juxtaposed in the base-6
 *            representation of k but not of k+1
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool has_adjacent_0_3(seq_t x) {
    /* return true if x contains the digits 0 and 3 next to each other in its base-6 representation */
    if (x < 6) return false;          /* single-digit numbers cannot contain an adjacency      */
    int last = (int)(x % 6);           /* least significant digit                           */
    x /= 6;                           /* remove that digit                                 */
    while (x > 0) {
        int cur = (int)(x % 6);        /* next more significant digit                       */
        if ((cur == 0 && last == 3) || (cur == 3 && last == 0))
            return true;              /* adjacency found                                   */
        last = cur;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n) {
    /*  n is 1-based; a(1) is the first term of the sequence */
    if (n == 0) return 0;              /* safety, not part of the official offset           */
    idx_t found = 0;
    seq_t candidate = 0;

    for (;;) {                          /* iterate over all integers starting at 0         */
        if (has_adjacent_0_3(candidate) && !has_adjacent_0_3(candidate + 1)) {
            found++;
            if (found == n)            /* desired term reached */
                return candidate;
        }
        candidate++;                    /* next integer                      */
    }
}
