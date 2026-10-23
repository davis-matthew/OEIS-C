/*
 * A117717  Maximal number of regions obtained by a straight line drawing of
 *          the complete bipartite graph K_{n,n}.
 * https://oeis.org/A117717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^2*(n^2+4)/4
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A117717: maximal regions of K_{n,n} */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, guard clause */
    seq_t ns  = (seq_t)n;                  /* cast index to seq_t */
    seq_t nm1 = ns - (seq_t)1;             /* n-1 */
    seq_t part1 = nm1 * nm1;               /* (n-1)^2 */
    seq_t part2 = ns * ns + (seq_t)4;      /* n^2 + 4 */

    /* Divide by 4 early to keep intermediate values small */
    if ((part1 % (seq_t)4) == 0)
        part1 /= (seq_t)4;
    else
        part2 /= (seq_t)4;

    return part1 * part2;                  /* final result */
}
