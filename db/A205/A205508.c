/*
 * A205508  a(n) = Pell(n) * A004018(n) for n>=1 with a(0)=1, where A004018(n)
 *          is the number of ways of writing n as a sum of 2 squares.
 * https://oeis.org/A205508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * A004018(n) for n>=1 (a(0)=1) * Pell(n) :
 *            Pell(0)=0, Pell(1)=1, Pell(n)=2*Pell(n-1)+Pell(n-2) *
 *            A004018(n): number of (ordered) representations of n as a^2 +
 *            b^2 * (n=0 → 1 representation: 0^2+0^2) * Implementation uses
 *            the classical factorisation formula: * if n contains a prime...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   representations_of_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/representations_of_two_squares.h"

/* Forward declare helper for integer square root (may be provided).
 * If not available, a simple implementation is provided later.
 */
#ifndef _floor_isqrt_u64
static inline idx_t _floor_isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (idx_t bit = ((idx_t)1 << ((sizeof(idx_t) * 8 - 1) & ~1)); bit; bit >>= 2) {
        idx_t next = r + bit;
        if (next <= n / next) r = next;
    }
    return r;
}
#endif

/* Computes the n-th Pell number */
static seq_t pell(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t p0 = 0, p1 = 1, pn = 0;
    for (idx_t i = 2; i <= n; ++i) {
        pn = 2 * p1 + p0;
        p0 = p1;
        p1 = pn;
    }
    return pn;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t p = pell(n);
    seq_t r = representations_of_two_squares(n);
    return p * r;
}
