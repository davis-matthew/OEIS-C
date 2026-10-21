/*
 * A211519  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w=2x-3y.
 * https://oeis.org/A211519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n-1)^2)/4) + floor((n-2)/3) + 1 (offset 1)
 * Range:     u8 n <= 16, u16 n <= 295, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0. */
    if (n <= 1)
        return 0;

    seq_t nn = (seq_t)n - 1;          /* n-1 */
    seq_t sq = nn * nn;               /* (n-1)^2 */
    seq_t part1 = sq / 4;             /* floor((n-1)^2/4) */

    seq_t part2 = ((seq_t)n - 2) / 3; /* floor((n-2)/3), n>=2 so non‑negative */

    return part1 + part2 + 1;         /* final value */
}
