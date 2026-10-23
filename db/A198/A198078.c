/*
 * A198078  a(n) = round((n+1/4)^n).
 * https://oeis.org/A198078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n + 1/4)^n) = round((4*n+1)^n / 4^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute (4*n+1)^n and 4^n.  The rounding is performed by adding
       half of the denominator before division.  When seq_t is 8 bytes or
       smaller we use 128‑bit arithmetic to avoid overflow for all n
       whose result still fits in seq_t.  For larger seq_t types the
       helper functions big_pow and pow2 provide arbitrary‑precision
       operation. */
    if (sizeof(seq_t) <= 8 && n <= 31) {
        unsigned __int128 base   = ((unsigned __int128)4 * (unsigned __int128)n) + 1;
        unsigned __int128 numer  = 1;
        for (idx_t i = 0; i < n; ++i)
            numer *= base;

        /* 4^n = 2^(2*n), ½*4^n = 2^(2*n-1) */
        unsigned __int128 denom  = ((unsigned __int128)1) << (2 * n);
        unsigned __int128 half   = ((unsigned __int128)1) << (2 * n - 1);

        unsigned __int128 result = (numer + half) / denom;
        return (seq_t)result;
    } else {
        seq_t base = (seq_t)n * (seq_t)4 + 1;
        seq_t numer = big_pow(base, n);
        seq_t denom = pow2(2 * n);
        seq_t half  = pow2(2 * n - 1);
        return (numer + half) / denom;
    }
}
