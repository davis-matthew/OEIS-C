/*
 * A128803  a(n) = n*(n-1)*9^n.
 * https://oeis.org/A128803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*9^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 977 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    /* compute 9^n */
    seq_t pow = ipow((seq_t)9, (seq_t)n);
    /* compute n*(n-1) in seq_t */
    seq_t small = (seq_t)n * ((seq_t)n - 1);
    return small * pow;
}
