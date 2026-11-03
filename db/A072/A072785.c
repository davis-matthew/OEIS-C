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
 * Validated: partial, pass, 112 terms through n = 111 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer header for uint64_t */
#include <stdint.h>

/* Compute the triangular index t for a given n.
   t satisfies: t*(t+1)/2 <= n < (t+1)*(t+2)/2 */
static inline idx_t triangular_index(idx_t n) {
    /* If n == 0, t = 0 */
    if (n == (idx_t)0)
        return 0;

    /* Choose a high bound by doubling until it's too big */
    unsigned __int128 high = 1;
    while ( (high * (high + 1)) / 2 <= (unsigned __int128)n )
        high <<= 1;

    unsigned __int128 low = high >> 1;
    idx_t best = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 val = (mid * (mid + 1)) / 2;
        if (val <= (unsigned __int128)n) {
            best = (idx_t)mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* avoid unsigned underflow */
            high = mid - 1;
        }
    }
    return best;
}

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
