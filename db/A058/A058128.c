/*
 * A058128  a(1) = 1, a(n) = (n^n-n)/(n-1)^2 for n >= 2.
 * https://oeis.org/A058128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula : a(n) = (n^n - n) / (n-1)^2  (n >= 2), a(1) = 1 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* 64‑bit implementation – use 128‑bit intermediate to avoid
       overflow for the few values that fit in uint64_t.  For n > 26
       the result overflow uint64_t anyway, so we simply fall back
       to the arbitrary‑precision path. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* 17^17 < 2^128, so we can safely use __int128 for n <= 26. */
        if (n <= 26) {
            unsigned __int128 base = (unsigned __int128)n;
            unsigned __int128 pow = 1;
            for (idx_t i = 0; i < n; ++i) pow *= base;

            unsigned __int128 num = pow - base;    /* n^n - n */
            unsigned __int128 denom = (unsigned __int128)(n - 1);
            denom = denom * denom;                 /* (n-1)^2 */

            return (seq_t)(num / denom);
        }
    }

    /* Generic implementation – works for arbitrary‑precision seq_t
       and for 64‑bit seq_t when n is small enough for the result to
       fit (the compiler will generate the correct code path below). */
    seq_t base = (seq_t)n;
    seq_t pow_val = big_pow(base, (seq_t)n);      /* n^n  */
    seq_t numerator = pow_val - (seq_t)n;
    seq_t denom = (seq_t)(n - 1);
    denom = denom * denom;                         /* (n-1)^2 */
    return numerator / denom;
}
