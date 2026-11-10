/*
 * A086068  Min{k: A086066(k) = n}.
 * https://oeis.org/A086068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = Min{k : A086066(k) = n}; the smallest number whose set of
 *            decimal digits is the set of bit positions of n (digits
 *            ascending, with 0 placed after the smallest nonzero digit)
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: full, pass, 1023 terms through n = 1023 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned mask = (unsigned)(n & 1023u);
    seq_t result = 0;
    int first = -1;
    int d;
    if (mask == 1u) {
        return (seq_t)0;
    }
    if (mask & 1u) {
        /* smallest nonzero digit goes first, then 0 */
        for (d = 1; d <= 9; d++) {
            if (mask & (1u << d)) { first = d; break; }
        }
        if (first < 0) return (seq_t)0;
        result = (seq_t)first;
        result = result * 10;
        for (d = first + 1; d <= 9; d++) {
            if (mask & (1u << d)) {
                result = result * 10 + (seq_t)d;
            }
        }
        return result;
    }
    for (d = 1; d <= 9; d++) {
        if (mask & (1u << d)) {
            result = result * 10 + (seq_t)d;
        }
    }
    return result;
}
