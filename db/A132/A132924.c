/*
 * A132924  Triangle read by columns, 2^(n-1) followed by (2^(n-1) + 1),
 *          (2^(n-1) + 2), (2^(n-1) + 3), ...
 * https://oeis.org/A132924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r + (2^(c-1) - c) where r = floor( ceil( sqrt(8*n+1) ) /
 *            2 ) c = n - r*(r-1)/2 (1 <= c <= r)
 * Range:     u8 n <= 7, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t) */
#include "../utils/pow_seq.h"      /* seq_t pow_seq(seq_t, int) */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset 1, but n>=1 in all tests */

    /* t = ceil( sqrt(8*n + 1) ) */
    seq_t val = (seq_t)8 * n + 1;
    seq_t t = floor_sqrt(val);
    if (t * t < val) t++;          /* ceil */

    idx_t r = (idx_t)(t / 2);      /* row number */

    /* starting index of row r : T(r-1) = r*(r-1)/2 */
    seq_t prev = (seq_t)r * (r - 1) / 2;
    seq_t k = (seq_t)n - prev;     /* 1‑based column within the row */
    idx_t c = (idx_t)k;

    seq_t pow2 = pow_seq((seq_t)2, (int)(c - 1)); /* 2^(c-1) */
    seq_t offset = pow2 - (seq_t)c;

    return (seq_t)r + offset;
}
