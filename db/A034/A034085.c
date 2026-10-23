/*
 * A034085  Decimal part of n-th root of a(n) starts with digit 8.
 * https://oeis.org/A034085
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 8          for n = 2
 *           a(n) = ceil( (9/5)^n ) for n >= 3
 *                = (9^n + 5^n - 1) / 5^n    (integer arithmetic)
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* provides big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 2)
        return 8;

    /* For 64‑bit builds compute using 128‑bit intermediates.
       For arbitrary‑precision builds use the provided big_pow routine. */
    if (sizeof(seq_t) == 8) {
        unsigned __int128 pow9 = 1;
        unsigned __int128 pow5 = 1;
        for (idx_t i = 0; i < n; ++i) {
            pow9 *= 9;
            pow5 *= 5;
        }
        unsigned __int128 numer = pow9 + pow5 - 1;
        unsigned __int128 denom = pow5;
        return (seq_t)(numer / denom);
    } else {
        seq_t pow9 = big_pow((seq_t)9, (seq_t)n);
        seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
        seq_t numer = pow9 + pow5 - (seq_t)1;
        return numer / pow5;
    }
}
