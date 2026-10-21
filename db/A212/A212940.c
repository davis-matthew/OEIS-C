/*
 * A212940  Number of nX5 0..4 arrays with no column j greater than or equal
 *          to than column j-1 in all rows.
 * https://oeis.org/A212940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*350^n+3*875^n+3*1125^n+3125^n+126^n-4*1875^n-2*525^n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Number of n×5 arrays over {0,1,2,3,4} with each row strictly decreasing
 * from left to right (so column j < column j‑1 for all j).  The sequence
 * satisfies the closed formula above. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset, harmless */

    seq_t sn = (seq_t)n;

    /* Compute the required powers */
    seq_t p350  = big_pow((seq_t)350,  sn);
    seq_t p875  = big_pow((seq_t)875,  sn);
    seq_t p1125 = big_pow((seq_t)1125, sn);
    seq_t p3125 = big_pow((seq_t)3125, sn);
    seq_t p126  = big_pow((seq_t)126,  sn);
    seq_t p1875 = big_pow((seq_t)1875, sn);
    seq_t p525  = big_pow((seq_t)525,  sn);

    /* Build the positive and negative parts of the expression */
    seq_t pos = (seq_t)3 * p875 + (seq_t)3 * p1125 + p3125 + p126;
    seq_t neg = (seq_t)2 * p350 + (seq_t)4 * p1875 + (seq_t)2 * p525;

    /* The formula guarantees pos ≥ neg; the result is positive. */
    return pos - neg;
}
