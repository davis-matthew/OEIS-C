/*
 * A183885  Number of arrangements of n+3 numbers in 0..2 with each number
 *          being the sum mod 3 of three others.
 * https://oeis.org/A183885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n+3) - 2*(n+4)^2 for n >= 4; a(1) = 1, a(2) = 121,
 *            a(3) = 631
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 62,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

/* Compute term a(n) of OEIS A183885 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    switch (n)
    {
        case 1: return (seq_t)1;
        case 2: return (seq_t)121;
        case 3: return (seq_t)631;
        default:
            {
                seq_t pow   = pow3((idx_t)(n + 3));
                seq_t t     = (seq_t)(n + 4);
                seq_t corr  = (seq_t)2 * t * t;   /* 2*(n+4)^2 */
                return pow - corr;
            }
    }
}
