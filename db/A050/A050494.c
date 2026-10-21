/*
 * A050494  Partial sums of A051923.
 * https://oeis.org/A050494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6, 6)*(3*n+7)/7
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 65, u64 n <= 1629,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute C(n+6,6) multiplicatively to avoid intermediate overflow. */
static seq_t binom6(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        /* multiply by (n+7-i), then divide by i.
           The division is always exact. */
        res = res * (seq_t)(n + 7 - i);
        res = res / (idx_t)i;
    }
    return res;
}

/* OEIS A050494 */
seq_t a(idx_t n)
{
    seq_t bin = binom6(n);                     /* C(n+6,6)      */
    seq_t mul = (seq_t)(3 * n + 7);            /* 3n+7          */

    /* At least one of bin or mul is divisible by 7.
       Dividing early keeps intermediate values within
       the limits of seq_t. */
    if (bin % 7 == 0) {
        bin /= 7;
    } else {
        mul /= 7;                  /* mul is guaranteed to be divisible by 7 here */
    }

    return bin * mul;
}
