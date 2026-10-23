/*
 * A047390  Numbers that are congruent to {0, 3, 5} mod 7.
 * https://oeis.org/A047390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1) + ceil((n-1)/3) (offset 1)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer arithmetic: a(n) = 2*(n-1) + (n+1)/3 */

seq_t a(idx_t n)
{
    if (n == 0u)                 /* safety: sequence is defined for n>=1 */
        return 0u;

    seq_t nn   = (seq_t)n;        /* cast index to seq_t for arithmetic */
    seq_t term = 2u * (nn - 1u) + (nn + 1u) / 3u;
    return term;
}
