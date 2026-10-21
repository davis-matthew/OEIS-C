/*
 * A059938  Sum of binary numbers with n 1's and two (non-leading) 0's.
 * https://oeis.org/A059938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*2^(n+1) - n*(n-1)/2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+1) as an unsigned multiplication loop
       to avoid undefined behaviour from shifting more than
       the width of seq_t. */
    seq_t power = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        power *= 2;

    seq_t n_seq = (seq_t)n;

    /* part1 = n^2 * 2^(n+1) */
    seq_t part1 = power * (n_seq * n_seq);

    /* part2 = n*(n-1)/2  (zero for n=0 or n=1) */
    seq_t part2 = (n_seq * (n_seq - 1)) / 2;

    /* the difference is the answer */
    return part1 - part2;
}
