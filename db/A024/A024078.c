/*
 * A024078  a(n) = 7^n - n^3.
 * https://oeis.org/A024078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - n^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses big_pow() to compute 7^n, then subtracts n^3.
 * Works for any seq_t type (built‑in unsigned or arbitrary‑precision).
 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);   /* 7^n */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;                   /* n^2 */
    seq_t n3   = n2 * nseq;                     /* n^3 */
    return pow7 - n3;
}
