/*
 * A197972  a(n) = ceiling((n+1/3)^n).
 * https://oeis.org/A197972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n+1/3)^n) = ((3n+1)^n + 3^n - 1) / 3^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1; for n == 0, return 1 via the formula. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* For small seq_t types (≤ 8 bytes) use 128‑bit intermediates
       to avoid overflow while still fitting the required range.   */
    if (sizeof(seq_t) <= 8) {
        /* Base and denominator in 128‑bit to hold intermediate values. */
        unsigned __int128 base   = (unsigned __int128)(3 * n + 1);
        unsigned __int128 num    = 1;
        unsigned __int128 den    = 1;

        for (idx_t i = 0; i < n; ++i) {
            num *= base;
            den *= (unsigned __int128)3;
        }

        unsigned __int128 res = (num + den - 1) / den;
        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t, rely on seq_pow. */
    seq_t base = (seq_t)(3 * n + 1);
    seq_t den  = seq_pow((seq_t)3, n);
    seq_t num  = seq_pow(base, n);

    seq_t tmp = num + den - (seq_t)1;
    return tmp / den;
}
