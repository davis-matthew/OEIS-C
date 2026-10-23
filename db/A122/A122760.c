/*
 * A122760  Triangle read by rows: t(n,m) = 2*3^m*(n mod 2).
 * https://oeis.org/A122760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Block-based construction of A122760.
   A(n) is 0 if the block index j with j*(j+1)/2 >= n is odd;
   otherwise A(n) = 2 * 3^{ n - (j-1)*j/2 - 1 } */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t j = 0;
    idx_t s = 0;
    while (s < n) {
        ++j;
        s += j;
    }
    idx_t prev_s = s - j;
    idx_t k = n - prev_s - 1;      /* 0‑based position in the even block */
    if (j & 1ULL)                  /* j odd → all zeros in this block */
        return (seq_t)0;
    return (seq_t)2 * pow3(k);
}
