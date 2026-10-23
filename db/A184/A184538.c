/*
 * A184538  Floor[1/{(3+n^4)^(1/4)}], where {}=fractional part.
 * https://oeis.org/A184538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*n^3/3) for n ≥ 3 a(1) = 2, a(2) = 11 The sequence
 *            is defined by a(n)=floor[1/{(3+n^4)^(1/4)}] and satisfies the
 *            above closed form for all n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two special cases */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;

    /* compute floor(4*n^3/3) using only integer arithmetic */
    seq_t t = (seq_t)n;        /* n                 */
    t   = t * t;               /* n^2               */
    t   = t * (seq_t)n;        /* n^3               */
    t   = t * 4;               /* 4 * n^3           */
    return t / 3;              /* integer division desc. */
}
