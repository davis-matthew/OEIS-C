/*
 * A166132  a(n) = 1 + (4*9^n - 9*4^n) / 5.
 * https://oeis.org/A166132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 49*a(n-2) + 36*a(n-3); Initial terms: a(1)=1,
 *            a(2)=37, a(3)=469
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; undefined for n==0 */
    if (n == 1) return 1;
    if (n == 2) return 37;
    if (n == 3) return 469;

    seq_t a1 = 1;      /* a(1) */
    seq_t a2 = 37;     /* a(2) */
    seq_t a3 = 469;    /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(n) = 14*a(n-1) - 49*a(n-2) + 36*a(n-3) */
        next = 14 * a3 - 49 * a2 + 36 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
