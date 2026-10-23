/*
 * A020338  Doublets: base-10 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n * 10^(digit_count(n)) + n (concatenation n||n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset starts at 1; return 0 for the undefined value n==0 */
        return (seq_t)0;
    }
    /* number of decimal digits of n */
    idx_t d = (idx_t)digit_count((uint64_t)n);
    /* 10^d as seq_t */
    seq_t pow10d = pow10(d);
    /* concatenate n with itself: n * 10^d + n */
    return ((seq_t)n) * pow10d + (seq_t)n;
}
