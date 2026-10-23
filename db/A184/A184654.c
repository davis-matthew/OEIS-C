/*
 * A184654  floor(n*sqrt(3)-2/3); complement of A184655.
 * https://oeis.org/A184654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3) - 2/3)
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t nsq  = nseq * nseq;                 /* n² */

    /* t  = ⌊n*√3⌋ */
    seq_t t    = floor_sqrt((seq_t)3 * nsq);

    /* s3 = ⌊3n*√3⌋  == ⌊√(27·n²)⌋ */
    seq_t s3   = floor_sqrt((seq_t)27 * nsq);

    /* diff = s3 – 3*t = ⌊3f⌋ where f is the fractional part of n√3  */
    seq_t diff = s3 - (seq_t)3 * t;

    /* If the fractional part ≥ 2/3 i.e. s3–3t ≥ 2, keep t; else, subtract 1 */
    if (diff >= (seq_t)2)
        return t;
    else
        return t - (seq_t)1;
}
