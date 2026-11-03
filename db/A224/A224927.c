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
 * Range:     u8 n <= 16, u16 n <= 1718, u32 n <= 11000, u64 n <= 11000,
 *            u128 n <= 11000, big any n
 * Validated: standard, pass, 89 terms through n = 8893 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Number of decimal digits of n. */
    idx_t k = num_digits_idx(n);
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
