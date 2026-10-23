/*
 * A198077  Round((n+1/3)^n).
 * https://oeis.org/A198077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n + 1/3)^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Round((n+1/3)^n) = Round((3n+1)^n / 3^n).  */
/* For seq_t <= 64 bits and n <= 21 a 128‑bit intermediate is used
 * to avoid overflow; otherwise the arbitrary‑precision power routine
 * from utils/pow_seq.h is employed. */

#include <stdint.h>                  /* for uint64_t */
#include "../utils/pow_seq.h"         /* seq_t pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1; return 0 for safety */
        return (seq_t)0;

    /* Small n on 64‑bit builds: use 128‑bit arithmetic */
    if (sizeof(seq_t) <= sizeof(uint64_t) && n <= 21)
    {
        __int128 base   = (__int128)3 * n + 1;   /* 3n+1 */
        __int128 denom  = 1;
        for (idx_t i = 0; i < n; ++i) denom *= 3;
        __int128 numer  = 1;
        for (idx_t i = 0; i < n; ++i) numer *= base;

        __int128 half   = denom / 2;               /* floor(denom/2) */
        __int128 r      = (numer + half) / denom;   /* rounded value */
        return (seq_t)r;                           /* fits into seq_t */
    }

    /* General case: use arbitrary‑precision power routine */
    seq_t base   = (seq_t)3 * (seq_t)n + (seq_t)1;
    seq_t denom  = pow_seq((seq_t)3, (int)n);
    seq_t numer  = pow_seq(base, (int)n);
    seq_t half   = denom / (seq_t)2;
    seq_t result = (numer + half) / denom;
    return result;
}
