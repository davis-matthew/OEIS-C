/*
 * A080455  a(1)=1; for n>1, a(n) = a(n-1) if n is already in the sequence,
 *          a(n) = a(n-1) + 4 otherwise.
 * https://oeis.org/A080455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + 1 - 4*floor((n+3)/4) (offset 1)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index */
    idx_t q = (n + 3) / 4;          /* floor((n+3)/4) */
    seq_t part1 = (seq_t)n * 4 + 1; /* 4*n + 1   */
    seq_t part2 = (seq_t)q * 4;     /* 4*floor(...) */

    return part1 - part2;
}
