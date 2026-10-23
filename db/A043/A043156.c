/*
 * A043156  Numbers k such that 0 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A043156: Numbers k such that 0 and 5 occur juxtaposed in the base-8 representation of k but not of k-1.
   Formula: a(n) = the n-th integer k > 0 where 
                (k has a 0-5 or 5-0 adjacency in base‑8) AND
                (k-1 does NOT have such an adjacency)
*/
#include <stdbool.h>

static inline bool has_adjacent(seq_t k) {
    bool adj = false;
    int prev = -1;                    /* sentinel for the previous digit */
    while (k != 0) {
        int cur = (int)(k % 8);       /* least significant octal digit   */
        if (prev != -1 && ((cur == 0 && prev == 5) || (cur == 5 && prev == 0))) {
            adj = true;
        }
        prev = cur;
        k /= 8;                       /* move to next octal digit        */
    }
    return adj;
}

seq_t a(idx_t n) {
    seq_t count = 0;   /* number of qualifying integers found so far */
    seq_t k = 1;       /* candidate integer */

    while (1) {
        if (has_adjacent(k) && !has_adjacent(k - 1)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
