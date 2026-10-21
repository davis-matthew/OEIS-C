/*
 * A052380  a(n) = D is the smallest distance (D) between 2 non-overlapping
 *          prime twins differing by d=2n; these twins are [p,p+d] or
 *          [p+D,p+D+d] and p > 3.
 * https://oeis.org/A052380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * ceil(n/3) (with offset 1, a(1)=6)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute ceil(n/3) as (n+2)/3 */
    seq_t t = ((seq_t)n + 2) / 3;
    return t * 6;
}
