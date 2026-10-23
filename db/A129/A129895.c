/*
 * A129895  a(1)=1. a(n) = a(n-1) + number of triangular numbers among the
 *          first (n-1) terms of the sequence.
 * https://oeis.org/A129895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4*⌊n/8⌋ + (n mod 8)) * (2*⌊n/8⌋ + 1)) + offset, where
 *            offset = 1 if (n mod 8)==0, offset = 0 if 1 ≤ (n mod 8) ≤ 3,
 *            offset = (n mod 8)−3 otherwise
 * Range:     u8 n <= 43, u16 n <= 722, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, dummy value */

    idx_t m = n / 8;                 /* floor division */
    idx_t k = n % 8;                 /* remainder */

    seq_t term1 = ((seq_t)4 * m) + (seq_t)k;
    seq_t term2 = ((seq_t)2 * m) + (seq_t)1;
    seq_t base  = term1 * term2;

    int offset;
    if (k == 0)
        offset = 1;
    else if (k <= 3)
        offset = 0;
    else
        offset = (int)(k - 3);

    return base + (seq_t)offset;
}
