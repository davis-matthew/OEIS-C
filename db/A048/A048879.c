/*
 * A048879  Generalized Pellian with second term of 10.
 * https://oeis.org/A048879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2); a(0) = 1, a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 10;

    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 10;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = 4 * a1 + a0;  /* recurrence */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
