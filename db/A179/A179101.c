/*
 * A179101  Numbers which are not the sum of exactly one positive square and a
 *          sum of squares-minus-1.
 * https://oeis.org/A179101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer NOT representable as 1+3k, 9+3k, or 17+3k  (OEIS notation) */
#include <stdbool.h>

static inline bool is_representable(seq_t x) {
    /* Determine whether x can be written as 1+3k, 9+3k, or 17+3k with k>=0. */
    seq_t r = x % 3;
    if (r == 1) {            /* x = 1 + 3k  */
        return x >= 1;
    } else if (r == 0) {     /* x = 9 + 3k  */
        return x >= 9;
    } else {                 /* r == 2,  x = 17 + 3k  */
        return x >= 17;
    }
}

seq_t a(idx_t n) {
    /* Find the n-th integer that is not representable as specified. */
    idx_t pos = 0;      /* number of non‑representable ints found so far   */
    seq_t x   = 1;      /* candidate integer, start from 1 (offset = 1)    */

    while (1) {
        if (!is_representable(x)) {
            pos++;
            if (pos == n) {
                return x;
            }
        }
        x++;
    }
}
