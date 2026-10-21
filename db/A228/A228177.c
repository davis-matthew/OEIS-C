/*
 * A228177  Floor(n*(sqrt(6)+sqrt(5))).
 * https://oeis.org/A228177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(sqrt(6)+sqrt(5)) )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 232, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   leq_alpha
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a binary search with exact integer algebra.
 * It tests whether m <= n*alpha by comparing m^2 with 11*n^2 + 2*n^2*sqrt(30)
 * The inequalities avoid floating point and work for any integer type seq_t. */
#include <stdbool.h>
#include "../utils/leq_alpha.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t sn = (seq_t)n;               /* n as seq_t */
    seq_t low  = 0;
    seq_t high = sn * 6 + 10;          /* high bound > n*alpha for all n */
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        if (leq_alpha(mid, sn))
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* high is the largest m with m <= n*alpha */
}
