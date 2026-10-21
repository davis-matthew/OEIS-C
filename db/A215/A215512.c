/*
 * A215512  a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3), with a(0)=1, a(1)=3, a(2)=8.
 * https://oeis.org/A215512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3), a(0)=1, a(1)=3, a(2)=8
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 8;

    /* Iterate from 3 up to n, keeping only the last three terms */
    seq_t a0 = 1;          /* a(n-3) */
    seq_t a1 = 3;          /* a(n-2) */
    seq_t a2 = 8;          /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 5 * a2 - 6 * a1 + a0;   /* a(i) = 5*a(i-1) - 6*a(i-2) + a(i-3) */
        a0 = a1;                      /* shift window forward */
        a1 = a2;
        a2 = an;
    }

    return a2;
}
