/*
 * A072785  Differences between A072781 and A072738.
 * https://oeis.org/A072785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = floor((2*(n+1) - t*t - t)/(t+3)) * (-1)^t, where t =
 *            A003056(n) = floor((-1 + sqrt(8*n-7))/2)
 * Range:     i8 n <= 111, i16 n <= 111, i32 n <= 111, i64 n <= 111,
 *            i128 n <= 111, big any n
 * Helpers:   triangular_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer header for uint64_t */
#include <stdint.h>
#include "../utils/triangular_index.h"

seq_t a(idx_t n) {
    /* Compute the triangular index t for this n */
    idx_t t = triangular_index(n);

    /* Compute numerator: 2*(n+1) - t*t - t */
    seq_t num = (seq_t)2 * (((seq_t)n) + 1) - (seq_t)t * (seq_t)t - (seq_t)t;

    /* Denominator */
    seq_t denom = ((seq_t)t) + 3;

    /* Division truncates toward zero (floor for positive numerator) */
    seq_t val = num / denom;

    /* Apply sign based on parity of t */
    if (t & 1) {
        /* t is odd -> multiply by -1 */
        val = -val;
    }
    return val;
}
