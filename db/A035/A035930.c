/*
 * A035930  Maximal product of any two numbers whose concatenation is n.
 * https://oeis.org/A035930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = max_{1 <= k < digits(n)} (floor(n / 10^k)) * (n mod 10^k)
 * Range:     u8 n <= 298, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where digits(n) is the decimal digit length of n) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* No split possible for single‑digit numbers */
    if (n < 10) return 0;

    seq_t maxProd = 0;
    idx_t rightDiv = 10;               /* current divisor 10^k */

    while (rightDiv <= n) {
        idx_t right = n % rightDiv;    /* right part */
        idx_t left  = n / rightDiv;    /* left part  */

        seq_t prod = (seq_t)left * (seq_t)right;
        if (prod > maxProd) maxProd = prod;

        /* prepare for next split, ensuring no overflow of rightDiv */
        if (rightDiv > n / 10) break;   /* next rightDiv would exceed n */
        rightDiv *= 10;
    }
    return maxProd;
}
