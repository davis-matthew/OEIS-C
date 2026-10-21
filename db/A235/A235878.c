/*
 * A235878  Number of (n+1) X (2+1) 0..2 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * ( 2 + 3 * 2^(n+1) + 3^n ) (conjecture by Colin
 *            Barker, 2018)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes a(n) for the OEIS sequence A235878.
   The sequence is defined with offset 1; a(1) is the first term.
   For n < 1, a(0) is returned as 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset protection */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 1)); /* 2^(n+1) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);       /* 3^n   */
    seq_t part = (seq_t)2 + (seq_t)3 * pow2 + pow3;
    return (seq_t)3 * part;
}
