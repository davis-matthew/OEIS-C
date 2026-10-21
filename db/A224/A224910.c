/*
 * A224910  Positive integer solutions to 1/p + 1/q + 1/r = 1 by lexicographic
 *          order.
 * https://oeis.org/A224910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Sequence A224910 is finite. There are only three distinct
 *            positive integer solutions to 1/p + 1/q + 1/r = 1, namely
 *            (2,3,6), (2,4,4) and (3,3,3). The sequence lists the three
 *            entries of each solution in lexicographic order, giving 9 terms
 *            in total
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The 9 known terms, indexed from 0. */
    static const seq_t seq[] = {2, 3, 6, 2, 4, 4, 3, 3, 3};

    if (n < 9u)           /* 9 terms */
        return seq[(idx_t)n];

    /* For indices beyond the finite sequence, return 0. */
    return 0;
}
