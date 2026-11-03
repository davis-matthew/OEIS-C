/*
 * A086099  a(n) = OR(k AND (n-k): 0<=k<=n), AND and OR bitwise.
 * https://oeis.org/A086099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, hear, look
 * Formula:   a(n) = OR{k & (n-k) : 0<=k<=n} Recurrence: a(0) = 0 a(2*m) = 2 *
 *            2^⌊log2(m)⌋ - 1 a(2*m+1) = 2 * a(m)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9438 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, builtins */

static seq_t a_calc(idx_t n)          /* helper – recursive */
{
    if (n == 0) return 0;

    if ((n & 1ULL) == 0) {           /* even: n = 2*m  */
        idx_t m = n >> 1;            /* m >= 1 */
        unsigned l = 63 - __builtin_clzll(m);   /* ⌊log2(m)⌋ */
        seq_t t = (seq_t)1 << l;     /* 2^⌊log2(m)⌋ */
        return ((seq_t)2 * t) - 1;
    }
    /* odd: n = 2*m+1 → a(n) = 2 * a(m) */
    return a_calc(n >> 1) << 1;
}

seq_t a(idx_t n)
{
    return a_calc(n);
}
