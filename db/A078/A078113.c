/*
 * A078113  Let u(1)=u(2)=1, u(3)=n, u(k) = (1/2)*abs(2*u(k-1)
 *          -u(k-2)-u(k-3)); sequence gives values of n such that Sum_{k>=1}
 *          u(k) is an integer.
 * https://oeis.org/A078113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-1) + 2*a(n-2) – 2*a(n-3) * Initial values: a(1)=2,
 *            a(2)=6, a(3)=7 * (Recurrence derived from the OEIS definition.)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1 – handle calls below the offset safely. */
    if (n == 0) return 0;

    /* Direct access for the first three terms. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)7;

    /* Compute terms iteratively; keep only the last three values. */
    seq_t a1 = 2;   /* a(n-3) */
    seq_t a2 = 6;   /* a(n-2) */
    seq_t a3 = 7;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 + 2 * a2 - 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;    /* now holds a(n) */
}
