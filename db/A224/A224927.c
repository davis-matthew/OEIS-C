/*
 * A224927  Denominator of lexicographically least fraction f satisfying
 *          floor(f*10^A055642(n)) = n.
 * https://oeis.org/A224927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = denominator of the lexicographically least fraction f
 *            such that floor(f * 10^digits(n)) = n. The fraction is not
 *            required to be reduced; lexicographic order is by numerator then
 *            denominator
 * Range:     u8 n <= 16, u16 n <= 1718, u32 n <= 8893, u64 n <= 8893,
 *            u128 n <= 8893, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Number of decimal digits of n. */
    idx_t k = digits(n);
    /* 10^k as a seq_t. */
    seq_t pow10k = pow10(k);

    for (idx_t p = 1; ; ++p) {
        seq_t A = (seq_t)p * pow10k;           /* p * 10^k */
        /* Upper bound for q: floor( A / n ). Must be >= 1. */
        seq_t high = A / (seq_t)n;
        if (high == 0) continue;              /* should not happen for p >= 1 */

        /* Lower bound for q: floor( A / (n+1) ) + 1. */
        seq_t low  = A / (seq_t)(n + 1) + 1;

        if (low <= high) {
            /* low is the smallest denominator that works for this p. */
            return low;
        }
    }
}
