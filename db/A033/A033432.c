/*
 * A033432  a(n) = floor(1000/sqrt(n)).
 * https://oeis.org/A033432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1000 / sqrt(n))
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* 1000^2 = 1,000,000 */
    const seq_t denom = 1000000;
    /* n ≥ 1 (offset 1) */
    seq_t q = denom / (seq_t)n;       /* floor(denom / n) */
    seq_t k = floor_sqrt(q);         /* floor(sqrt(q)) = floor( sqrt(denom / n) ) */
    return k;
}
