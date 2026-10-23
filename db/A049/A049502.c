/*
 * A049502  Major index of n, 2nd definition.
 * https://oeis.org/A049502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum of (p+1) over all positions p (0‑based) such that
 * bit p of n is 1 and bit p+1 is 0, when n is written in binary and
 * positions are counted from the rightmost bit starting at 1.
 */
#include <stdint.h>

static inline idx_t bit_length(idx_t n)
{
    idx_t len = 0;
    while (n)
    {
        ++len;
        n >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    idx_t len = bit_length(n);
    if (len < 2)
        return sum;           /* no bit followed by 0 possible */

    /* Scan bits 0 … len-2 (the second most significant bit may not have a
     * following higher bit).  For each position p where bit p is 1 and bit
     * p+1 is 0, add the 1‑based position (p+1) to the sum. */
    for (idx_t p = 0; p < len - 1; ++p)
    {
        if (((n >> p) & 3) == 1)   /* pattern 01 in 2‑bit window = 10 in binary */
            sum += (seq_t)(p + 1);
    }

    return sum;
}
