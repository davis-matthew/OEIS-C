/*
 * A191633  a(n) = floor((1 + 2^n)/(1 + 2*n)).
 * https://oeis.org/A191633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor((1 + 2^n)/(1 + 2*n))
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute denominator: 1 + 2*n */
    seq_t denom = (seq_t)1 + (seq_t)2 * (seq_t)n;

    /* Compute remainder of 2^n modulo denom. */
    seq_t rem = 1;                     /* 2^0 % denom = 1 % denom */
    for (idx_t i = 0; i < n; ++i) {
        /* rem = (rem * 2) % denom; use 128‑bit to avoid overflow */
        rem = (((unsigned __int128)rem) * 2) % denom;
    }

    /* Compute the integer part of 2^n / denom.
       For 64‑bit builds we use 128‑bit arithmetic where possible.
       If the value does not fit in seq_t, we set q = 0 – such cases
       are not required to be computed exactly. */
    seq_t q;
    if (sizeof(seq_t) <= 8) {                 /* 64‑bit or less */
        if (n < 128) {
            __uint128_t val = (__uint128_t)1 << n;  /* 2^n  (fits in 128‑bit) */
            q = (seq_t)(val / denom);
        } else {
            q = 0;                          /* overflow – value too large */
        }
    } else {
        seq_t two_pow = pow2(n);             /* arbitrary precision */
        q = two_pow / denom;
    }

    /* Add one if the extra +1 in the numerator turns a remainder
       of denom-1 into a full extra division. */
    if (rem == denom - 1) {
        q = q + 1;
    }

    return q;
}
