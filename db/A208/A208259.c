/*
 * A208259  Numbers starting and ending with digit 1.
 * https://oeis.org/A208259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose decimal representation
 *            starts and ends with 1
 * Range:     u8 n <= 12, u16 n <= 1112, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_seq.h"
/* Computes 10^e in seq_t */

seq_t a(idx_t n)
{
    /* Find the length L of the nth number */
    idx_t L = 1;          /* current length */
    idx_t cnt = 1;        /* how many numbers have length L */
    idx_t cum_prev = 0;   /* cumulative count up to length L-1 */

    while (1) {
        idx_t cum = cum_prev + cnt;
        if (cum >= n)
            break;
        cum_prev = cum;
        ++L;
        /* After the first two lengths, each new length adds a factor of 10 */
        if (L >= 3)
            cnt *= 10;
    }

    if (L == 1)
        return (seq_t)1;   /* the only one-digit number is 1 */

    idx_t offset = n - cum_prev - 1;   /* zero‑based index within this length block */

    seq_t pow10full = pow10_seq(L - 1);   /* 10^(L-1) */
    seq_t result = pow10full + (seq_t)offset * 10 + 1;
    return result;
}
