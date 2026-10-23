/*
 * A162439  Write down the binary representation of n. Partition the string
 *          which is this binary representation by placing a '+' just left of
 *          every 1. Add the resulting base 2 numbers. a(n) = decimal
 *          equivalent of this sum.
 * https://oeis.org/A162439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   For n with binary expansion n = Σ 2^k_i (k_1>k_2>…>k_r), a(n) =
 *            2^(k_1−k_2−1) + … + 2^(k_{r−1}−k_r−1) + 2^k_r
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* defensively return 0 for n==0 */

    /* collect the positions of all set bits of n, 0‑based, least‑significant first */
    idx_t bits[64];
    idx_t cnt = 0;
    idx_t pos = 0;
    for (idx_t tmp = n; tmp; ++pos, tmp >>= 1) {
        if (tmp & 1)
            bits[cnt++] = pos;
    }

    /* compute a(n) using the formula */
    seq_t sum = ((seq_t)1 << bits[0]);          /* 2^k_r  (k_r = lowest set bit) */
    for (idx_t i = 1; i < cnt; ++i) {
        idx_t diff = bits[i] - bits[i - 1] - 1; /* k_i - k_{i-1} - 1   */
        sum += ((seq_t)1 << diff);             /* add 2^diff          */
    }
    return sum;
}
