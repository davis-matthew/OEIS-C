/*
 * A016247  Expansion of 1/((1-x) * (1-6*x) * (1-11*x)).
 * https://oeis.org/A016247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 66*a(n-2) + 1 (a(0)=1, a(1)=18)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 18;

    seq_t a0 = 1;                     /* a(0)   */
    seq_t a1 = 18;                    /* a(1)   */
    seq_t an = 0;                     /* a(i)   */

    for (idx_t i = 2; i <= n; ++i) {
        an = 17 * a1 - 66 * a0 + 1;    /* recurrence */
        a0 = a1;
        a1 = an;
    }
    return an;
}
