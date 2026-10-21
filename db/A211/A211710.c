/*
 * A211710  Number of (n+1) X (n+1) -11..11 symmetric matrices with every 2 X
 *          2 subblock having sum zero and two distinct values.
 * https://oeis.org/A211710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=34, a(2)=46, a(3)=64, a(n)=2*a(n-1)-a(n-3) for n>=4
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 41, u64 n <= 87, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* the offset is 1, but return 0 for safety */
        return (seq_t)0;

    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)46;
    if (n == 3) return (seq_t)64;

    /* Work from the 1-based indices using a rolling window of
       the last three values a(n-3), a(n-2), a(n-1). */
    seq_t a_n_minus_3 = (seq_t)34;   /* a(1) */
    seq_t a_n_minus_2 = (seq_t)46;   /* a(2) */
    seq_t a_n_minus_1 = (seq_t)64;   /* a(3) */
    seq_t a_n;                       /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)2 * a_n_minus_1 - a_n_minus_3;
        /* shift the window for the next iteration */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    return a_n;
}
