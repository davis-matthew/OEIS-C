/*
 * A078616  a(n) = Sum_{k=0..n} A010815(k).
 * https://oeis.org/A078616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0. For m ≥ 1 let s = m*(3*m+1)/2. If s ≤ n ≤ s+2*m
 *            then a(n)=(-1)^m; otherwise a(n)=0 (n lies in the subsequent
 *            zero block of length m+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;

    for (idx_t m = 1; ; ++m)
    {
        /* beginning of the +/- block for this m */
        idx_t start      = m * (3 * m + 1) / 2;
        idx_t block_end  = start + 2 * m;          /* inclusive */

        if (n <= block_end)                       /* inside +/- block */
            return (seq_t)((m & 1) ? -1 : 1);

        idx_t zero_end = block_end + m + 1;        /* end of following zero block */

        if (n <= zero_end)                        /* inside zero block */
            return 0;
    }
}
