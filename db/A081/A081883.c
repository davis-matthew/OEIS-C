/*
 * A081883  Numbers n such that the number of steps needed to reach an integer
 *          starting with n-1/4 or n+1/4 and iterating the map
 *          x-->x*ceiling(x) is the same.
 * https://oeis.org/A081883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   steps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/steps.h"

/* Implementation of OEIS sequence A081883: numbers n such that
   the number of steps needed to reach an integer starting with
   n-1/4 or n+1/4 and iterating the map x → x * ceil(x) are equal. */

/* Returns true iff the number m satisfies the property of A081883. */
static inline bool is_special(idx_t m) {
    seq_t p1 = ((seq_t)m * 4) - 1;
    seq_t p2 = ((seq_t)m * 4) + 1;
    idx_t s1 = steps(p1);
    idx_t s2 = steps(p2);
    return s1 == s2;
}

/* Returns the n-th term of the sequence (a(1) = 5, a(2) = 9, ...). */
seq_t a(idx_t n) {
    idx_t found = 0;
    for (idx_t m = 1;; ++m) {
        if (is_special(m)) {
            ++found;
            if (found == n) {
                return (seq_t)m;
            }
        }
    }
}
