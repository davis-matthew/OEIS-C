/*
 * A048499  a(n) = 2^(n-1)*(6*n-10)+6.
 * https://oeis.org/A048499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3) (n >= 3), with a(0) = 1,
 *            a(1) = 2, a(2) = 10. This recurrence follows the definition of
 *            OEIS sequence A048499
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 10;

    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 2;   /* a(n-2) */
    seq_t a2 = 10;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 5 * a2;   /* 5*a(n-1) */
        cur -= 8 * a1;  /* - 8*a(n-2) */
        cur += 4 * a0;  /* + 4*a(n-3) */

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
