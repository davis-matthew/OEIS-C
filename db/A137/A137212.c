/*
 * A137212  a(n) = 5*a(n-1) - 5*a(n-2) - 3*a(n-3).
 * https://oeis.org/A137212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 3*a(n-3), with a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;

    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 0;   /* a(n-2) */
    seq_t a2 = 1;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = 5 * a2 - 5 * a1 - 3 * a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
