/*
 * A029711  a(n) = n^(n+1) + (n-1)^2.
 * https://oeis.org/A029711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+1) + (n-1)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n^(n+1) */
    seq_t pow_val = big_pow((seq_t)n, (seq_t)(n + 1));

    /* (n-1)^2  – special‑case n==0 to avoid unsigned underflow */
    seq_t sq;
    if (n == 0) {
        sq = 1;                    /* (-1)^2 = 1 in Z, but n-1 underflows in unsigned */
    } else {
        seq_t nm1 = (seq_t)(n - 1);
        sq = nm1 * nm1;
    }

    return pow_val + sq;
}
