/*
 * A178352  Number of collinear point triples in a 6 X 6 X 6 X ...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*8^n - (9*6^n)/2 + 12^n + (18^n)/2 + 10^n for n ≥ 1;
 *            a(0) = 0 (using integer arithmetic with arbitrary precision
 *            support)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute needed powers */
    seq_t pow8  = pow_seq((seq_t)8,  (int)n);
    seq_t pow6  = pow_seq((seq_t)6,  (int)n);
    seq_t pow12 = pow_seq((seq_t)12, (int)n);
    seq_t pow18 = pow_seq((seq_t)18, (int)n);
    seq_t pow10 = pow_seq((seq_t)10, (int)n);

    /* positive part of the formula */
    seq_t pos = ((seq_t)2 * pow8) + pow12 + pow10 + (pow18 / (seq_t)2);

    /* negative part of the formula */
    seq_t neg = ((pow6 * (seq_t)9) / (seq_t)2);

    return pos - neg;
}
