/*
 * A072012  a(n) = A072010(A072010(n)).
 * https://oeis.org/A072012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A072010(A072010(n)), where A072010 is completely
 *            multiplicative with A072010(2) = 2, A072010(p) = p+2 for p == 1
 *            (mod 4), A072010(p) = p-2 for p == 3 (mod 4)
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9541 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A072012_swap(seq_t m) {
    seq_t result = (seq_t)1;
    while (m % 2 == 0 && m > 0) {
        m /= 2;
        result *= 2;
    }
    seq_t p;
    for (p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t t;
            if ((int)(p % 4) == 1)
                t = p + 2;
            else
                t = p - 2;
            while (m % p == 0) {
                m /= p;
                result *= t;
            }
        }
    }
    if (m > 1) {
        seq_t t;
        if ((int)(m % 4) == 1)
            t = m + 2;
        else
            t = m - 2;
        result *= t;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t x = A072012_swap((seq_t)n);
    return A072012_swap(x);
}
