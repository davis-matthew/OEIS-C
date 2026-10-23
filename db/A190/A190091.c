/*
 * A190091  Number of rhombuses on a (n+1) X 3 grid.
 * https://oeis.org/A190091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 12*n - (n%2?5:4))/4 (for n ≥ 1)
 * Range:     u8 n <= 11, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1, value for n=0 is unused */

    seq_t nn = (seq_t)n;
    /* Compute numerator: n^2 + 12*n - (n%2?5:4) */
    seq_t num = nn * nn + nn * (seq_t)12
                - (n & 1 ? (seq_t)5 : (seq_t)4);

    return num / (seq_t)4;            /* integer division, exact */
}
