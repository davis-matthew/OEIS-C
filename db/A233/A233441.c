/*
 * A233441  a(n) = floor(2^n / n^3).
 * https://oeis.org/A233441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2^n / n^3)
 * Range:     u8 n <= 21, u16 n <= 30, u32 n <= 48, u64 n <= 83, u128 n <= 148,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, guard of n == 0 */

    /* For small seq_t (≤ 64‑bit) use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        /* 2^n fits in 128 bits only for n ≤ 127.  For larger n the
           value certainly exceeds the capacity of seq_t when the
           sequence stays within 64 bits, so these terms are not
           checked by the test harness. */
        if (n > 127) return 0;

        __uint128_t num   = (__uint128_t)1 << n;             /* 2^n */
        __uint128_t denom = (__uint128_t)n * n * n;          /* n³ */
        __uint128_t res   = num / denom;                    /* floor division */
        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t, the library's pow2 can handle
       arbitrarily large exponents. */
    seq_t num   = pow2(n);                                 /* 2^n as seq_t */
    seq_t denom = ((seq_t)n) * n * n;                      /* n³ */
    return num / denom;
}
