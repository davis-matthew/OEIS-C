/*
 * A132162  a(2n+1) = 3*a(2*n) - 4*n with a(0) = 1, a(1) = 3.
 * https://oeis.org/A132162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A132162 – nonn, look sequence.
 * 
 * Implementation uses the closed form:
 *   a(2n)   = 2n + 3^⌊log₃(2n+1)⌋
 *   a(2n+1) = 2n + 3^⌈log₃(2n+1)⌉
 *   (equivalently: a(2n+1) = 3*a(2n) – 4n)
 *
 * All calculations use the integer types seq_t and idx_t supplied by the
 * build system.  No floating point or external headers are required.
 */
#include <stdint.h>            /* for uintptr_t if needed */

static /* seq_t */ seq_t pow3(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)3;
    return r;
}

static /* idx_t */ idx_t floor_log3(idx_t v) {
    idx_t k = 0;
    seq_t p = 1;                /* 3^k      */
    while (p * 3 <= v) {        /* while 3^(k+1) ≤ v */
        p *= (seq_t)3;
        ++k;
    }
    return k;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    idx_t m = n >> 1;           /* floor(n/2) -> "n" in formulas */
    idx_t k = floor_log3((idx_t)(2 * m + 1));
    seq_t powk = pow3(k);

    if ((n & 1) == 0) {         /* even index: a(2m)   */
        return (seq_t)(2 * m) + powk;
    } else {                    /* odd index : a(2m+1) */
        return (seq_t)(2 * m) + pow3(k + 1);
    }
}
