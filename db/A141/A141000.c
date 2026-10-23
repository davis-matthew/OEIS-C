/*
 * A141000  Numbers k for which there is a solution to the Jumping Grasshopper
 *          game.
 * https://oeis.org/A141000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (11 - (-1)^n + 4*n)/2 for n>6, with initial values
 *            a(1)=0, a(2)=1, a(3)=4, a(4)=9, a(5)=13, a(6)=16. This also
 *            satisfies a(n)=a(n-1)+a(n-2)-a(n-3) for n>9
 * Range:     u8 n <= 60, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {0, 1, 4, 9, 13, 16};
    if (n <= 6)
        return init[n - 1];

    /* (-1)^n: 1 if n even, -1 if n odd */
    int minus_one_n = (n & 1) ? -1 : 1;

    /* Compute (11 - (-1)^n + 4*n)/2 */
    seq_t term = (seq_t)(11 - minus_one_n);      /* 10 or 12 */
    term += (seq_t)4 * (seq_t)n;                /* add 4*n */
    return term / 2;
}
