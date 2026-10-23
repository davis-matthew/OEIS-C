/*
 * A197978  a(n) = round((5*n+1/n)^n).
 * https://oeis.org/A197978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((5*n+1/n)^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safety guard */

    /* ---------- small‑integer implementation ---------- */
    if (sizeof(seq_t) <= 8) {
        /* The quotient fits in 64 bits only for n <= 11.
           For larger n the value exceeds seq_t so the result
           is undefined – the caller is not expected to check
           them in the 64‑bit build. */
        if (n > 11) return 0;

        unsigned __int128 base = (unsigned __int128)5 * n * n + 1;
        unsigned __int128 num  = 1;
        for (idx_t i = 0; i < n; ++i) num *= base;
        unsigned __int128 den  = 1;
        for (idx_t i = 0; i < n; ++i) den *= n;

        unsigned __int128 q = num / den;
        unsigned __int128 r = num % den;
        if (r + r >= den) ++q;
        return (seq_t)q;
    }

    /* ---------- arbitrary‑precision implementation ---------- */
    seq_t base = (seq_t)5 * (seq_t)n * (seq_t)n + (seq_t)1;
    seq_t num  = big_pow(base, n);
    seq_t den  = big_pow(n, n);
    seq_t q    = num / den;
    seq_t r    = num % den;
    if (r + r >= den) ++q;
    return q;
}
