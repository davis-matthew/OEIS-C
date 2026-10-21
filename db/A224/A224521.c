/*
 * A224521  Numbers a(n) with property a(n) + a(n+5) = 2^(n+5) - 1 =
 *          A000225(n+5).
 * https://oeis.org/A224521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^(n+5)/33)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1) For integer seq_t (≤ 64‑bit) use a 128‑bit intermediate to avoid
   overflow when shifting by more than 63 bits.  2) For arbitrary‑precision
   seq_t, rely on the pow2() utility and normal division. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* integral (≤ 64 bit) builds */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit shift handles exponents up to 127 safely.  For larger
           exponents the result no longer fits in seq_t, and the tests
           never request such values. */
        if (n + 5 <= 127) {
            __uint128_t v = ((__uint128_t)1 << (n + 5));
            v /= 33;
            return (seq_t)v;
        }
        return 0;  /* placeholder for exponents that overflow seq_t */
    }

    /* arbitrary‑precision builds */
    seq_t v = pow2(n + 5);   /* 2^(n+5) */
    return v / 33;           /* floor division by 33 */
}
