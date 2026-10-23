/*
 * A168677  Lexicographically earliest positive integer sequence such that no
 *          sum of consecutive terms is a positive power of 4.
 * https://oeis.org/A168677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1,1,1,5,1,1,1,9]_((n-1) mod 8)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* The sequence is periodic with period (1,1,1,5,1,1,1,9) */
    static const seq_t period[8] = {1, 1, 1, 5, 1, 1, 1, 9};
    idx_t pos = (n - 1) % 8;
    return period[pos];
}
