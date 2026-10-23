/*
 * A115146  Seventh convolution of A115140.
 * https://oeis.org/A115146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = –7, a(2) = 14, a(3) = –7, a(4) = a(5) = a(6) =
 *            0, a(7) = –1. For n ≥ 8: a(n) = a(n‑1) * 2*(n‑4)*(2*n‑9) /
 *            (n*(n‑7)) This recurrence is exact and is used for both 64‑bit
 *            and arbitrary‑precision builds. For 64‑bit seq_t the
 *            intermediate calculations are performed with signed __int1...
 * Range:     i8 n <= 9, i16 n <= 13, i32 n <= 22, i64 n <= 38, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base[8] = { (seq_t)1, (seq_t)-7, (seq_t)14, (seq_t)-7,
                               (seq_t)0, (seq_t)0,  (seq_t)0, (seq_t)-1 };

seq_t a(idx_t n)
{
    if (n < 8)                /* use the known initial values */
        return base[n];

    /* Special case for the first non‑zero recurrence step */
    if (n == 8) {
        seq_t res = base[7];
        seq_t num, den;
        num = (seq_t)2 * (seq_t)(n-4) * (seq_t)(2*n-9);
        den = (seq_t)n * (seq_t)(n-7);
        res = (res * num) / den;
        return res;
    }

    /* For 64‑bit seq_t use __int128 intermediate arithmetic */
    if (sizeof(seq_t) <= 8) {
        __int128 res = (__int128)base[7];
        for (idx_t i = 8; i <= n; ++i) {
            __int128 num = (__int128)2 * (i-4) * (2*i-9);
            __int128 den = (__int128)i * (i-7);
            res = (res * num) / den;
        }
        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t type, use seq_t operations directly */
    seq_t res = base[7];
    for (idx_t i = 8; i <= n; ++i) {
        seq_t num  = (seq_t)2;
        num *= (seq_t)(i-4);
        num *= (seq_t)(2*i-9);

        seq_t den  = (seq_t)i;
        den *= (seq_t)(i-7);

        res = (res * num) / den;
    }
    return res;
}
