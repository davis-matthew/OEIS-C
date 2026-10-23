/*
 * A124760  Number of rises for compositions in standard order.
 * https://oeis.org/A124760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the number of rises in the standard‑order composition of
 *            n (A066099). Equivalent to: for the binary expansion of n, let
 *            pos be the set of 1‑bit positions (1‑indexed), prepend 0, take
 *            successive differences p, then a(n) is the number of indices
 *            where p[i] > p[i+1]
 * Range:     u8 n <= 9746, u16 n <= 9746, u32 n <= 9746, u64 n <= 9746,
 *            u128 n <= 9746, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t x = n;
    idx_t i = 0;                /* bit index, 0‑based */
    idx_t prevPos = 0;          /* position of the previous 1‑bit */
    seq_t prevDiff = 0;         /* previous difference in p */
    bool first = true;          /* true until the first 1‑bit is seen */
    seq_t rises = 0;            /* result */

    while (x) {
        if (x & 1U) {
            idx_t curPos = i + 1;                    /* 1‑based position */
            seq_t diff = (seq_t)(curPos - prevPos);  /* p value */
            if (!first) {
                if (prevDiff > diff)
                    rises++;
            }
            prevDiff = diff;
            prevPos = curPos;
            first = false;
        }
        ++i;
        x >>= 1;
    }
    return rises;
}
