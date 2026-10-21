/*
 * A061874  |First digit - second digit + third digit - fourth digit ...| = 5.
 * https://oeis.org/A061874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   alt_abs_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A061874: a(n) is the n-th positive integer with
    |first digit - second digit + third digit - fourth digit …| = 5   (alternating sum of digits in absolute value equals 5)   */
#include <stdlib.h>
#include "../utils/alt_abs_sum.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but harmless */
    idx_t count = 0;
    seq_t cand = 0;
    while (1) {
        ++cand;                                       /* test next integer */
        /* convert seq_t to unsigned long long for digit processing */
        unsigned long long v = (unsigned long long)cand;
        if (alt_abs_sum(v) == 5) {                    /* satisfies the condition */
            ++count;
            if (count == n) return cand;
        }
    }
}
