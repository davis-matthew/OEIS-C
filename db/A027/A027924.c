/*
 * A027924  a(n) = least k such that 1+2+...+k >= 1^3 + 2^3 + ... + n^3.
 * https://oeis.org/A027924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that k(k+1)/2 ≥ (n(n+1)/2)² Equivalent to
 *            a(n) = round(√(½)·n(n+1)) which avoids floating point.
 *            Implementation uses a binary search on k, avoiding any external
 *            utility and requiring only arithmetic on seq_t
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for compatibility – not strictly needed */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence; handle n == 0 gracefully */
    if (n == 0) {
        return 0;
    }

    /* Convert n to seq_t for safe arithmetic */
    seq_t nn = (seq_t)n;

    /* S = n(n+1)/2, the sum of the first n integers */
    seq_t S = nn * (nn + (seq_t)1) / (seq_t)2;

    /* T = S², the sum of the first n cubes */
    seq_t T = S * S;

    /* Binary search for the minimal k with k(k+1)/2 ≥ T */
    seq_t low  = 0;
    seq_t high = S * (seq_t)2;   /* 2·S is a safe upper bound */

    while (low < high) {
        seq_t mid  = (low + high) / (seq_t)2;
        seq_t lhs  = mid * (mid + (seq_t)1) / (seq_t)2;

        if (lhs >= T) {
            high = mid;
        } else {
            low = mid + (seq_t)1;
        }
    }

    return low;
}
