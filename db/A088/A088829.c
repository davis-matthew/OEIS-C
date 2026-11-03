/*
 * A088829  Even numbers with even abundance.
 * https://oeis.org/A088829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9633 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088829: a(n) = the n-th even integer whose sum of divisors is even
 * (i.e., the odd part of n is not a perfect square). */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, just for safety */
    idx_t count = 0;
    seq_t cand   = 6;              /* first even number in the sequence */

    while (count < n) {
        seq_t m = cand;
        /* strip all factors of 2 */
        while ((m % 2) == 0) m /= 2;
        seq_t s = floor_sqrt(m);  /* integer square root of the odd part */
        if (s * s != m) {         /* odd part not a perfect square -> keep */
            ++count;
            if (count == n) return cand;
        }
        cand += 2;                 /* next even integer */
    }
    return 0; /* unreachable */
}
