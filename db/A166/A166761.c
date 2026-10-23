/*
 * A166761  Number of n X 3 1..2 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+5) - (2*n^3 + 9*n^2 + 73*n + 96)/3 for n ≥ 1
 *            (derived from a(n) = (3*2^(n+5) - 2*n^3 - 9*n^2 - 73*n - 96)/3)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;

    /* Compute 2^(n+5) */
    seq_t pow = pow2(n + 5);          /* pow = 2^(n+5) */

    /* Compute polynomial part: 2*n^3 + 9*n^2 + 73*n + 96 */
    seq_t n2 = nseq * nseq;           /* n^2 */
    seq_t n3 = n2 * nseq;             /* n^3 */

    seq_t term = (seq_t)2 * n3;        /* 2*n^3 */
    term += (seq_t)9 * n2;            /* +9*n^2 */
    term += (seq_t)73 * nseq;         /* +73*n */
    term += (seq_t)96;                /* +96 */

    /* Divide by 3 – the numerator is always divisible by 3 */
    seq_t sub = term / (seq_t)3;      /* (2*n^3 + 9*n^2 + 73*n + 96)/3 */

    /* Final result: 2^(n+5) - sub */
    return pow - sub;
}
