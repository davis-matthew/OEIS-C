/*
 * A220983  The left Aurifeuillian factor of 7^(14n+7) + 1.
 * https://oeis.org/A220983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(6n+3) - 7^(5n+3) + 3*7^(4n+2) - 7^(3n+2) + 3*7^(2n+1)
 *            - 7^(n+1) + 1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* compute the various powers of 7 needed in the formula */
    idx_t e1 = 6 * n + 3;
    idx_t e2 = 5 * n + 3;
    idx_t e3 = 4 * n + 2;
    idx_t e4 = 3 * n + 2;
    idx_t e5 = 2 * n + 1;
    idx_t e6 = n + 1;

    seq_t p1 = big_pow((seq_t)7, (seq_t)e1);
    seq_t p2 = big_pow((seq_t)7, (seq_t)e2);
    seq_t p3 = big_pow((seq_t)7, (seq_t)e3);
    seq_t p4 = big_pow((seq_t)7, (seq_t)e4);
    seq_t p5 = big_pow((seq_t)7, (seq_t)e5);
    seq_t p6 = big_pow((seq_t)7, (seq_t)e6);

    /* assemble the result using only integer arithmetic */
    seq_t res = p1 - p2 + 3 * p3 - p4 + 3 * p5 - p6 + 1;
    return res;
}
