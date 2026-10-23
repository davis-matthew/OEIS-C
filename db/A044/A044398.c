/*
 * A044398  Numbers n such that string 6,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose decimal representation contains
 *            the substring “66” while the previous integer does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: does v contain the decimal substring "66"? */
static bool has66(seq_t v) {
    if (v < 66) return false;          /* short‑circuit for small numbers */
    uint64_t last = (uint64_t)(v % 10); /* low digit */
    v /= 10;
    while (v > 0) {
        uint64_t d = (uint64_t)(v % 10);
        if (d == 6 && last == 6) return true;
        last = d;
        v /= 10;
    }
    return false;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for n=0, return 0 */
    idx_t count = 0;
    for (idx_t i = 1; ; ++i) {        /* start from 1 to avoid underflow */
        if (has66((seq_t)i)) {
            if (!has66((seq_t)(i - 1))) {
                ++count;
                if (count == n) return (seq_t)i;
            }
        }
    }
}
