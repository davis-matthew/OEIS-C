/*
 * A199986  Numbers with digital product = 2.
 * https://oeis.org/A199986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (10^k - 1)/9 + 10^r, where k = smallest integer with
 *            k(k+1)/2 ≥ n, and r = n - k(k-1)/2 - 1 (0 ≤ r < k)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 209, u128 n <= 780,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* n is guaranteed ≥ 1 in the OEIS range */

    /* Find the block length k: k*(k+1)/2 ≥ n. */
    idx_t k = 1;
    while (((seq_t)k * ((seq_t)k + 1)) / 2 < n)
        ++k;

    /* Number of terms before this block. */
    idx_t prevSum = ((k - 1) * k) / 2;

    /* 0‑based index inside the block. */
    idx_t r = n - prevSum - 1;   /* 0 ≤ r < k */

    /* Build the number consisting of k ones: 111…1. */
    seq_t ones = 0;
    for (idx_t i = 0; i < k; ++i)
        ones = ones * ((seq_t)10) + 1;

    /* Add 10^r to place the single digit 2. */
    seq_t pow10r = 1;
    for (idx_t i = 0; i < r; ++i)
        pow10r *= ((seq_t)10);

    return ones + pow10r;
}
