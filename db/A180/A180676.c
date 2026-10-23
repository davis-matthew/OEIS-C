/*
 * A180676  The Gi3 sums of the Pell-Jacobsthal triangle A013609.
 * https://oeis.org/A180676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4), a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)1;  /* a(2) */
    seq_t a3 = (seq_t)1;  /* a(3) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    seq_t ai; /* holds a(i) during iteration */

    for (idx_t i = 4; i <= n; ++i) {
        ai = (20 * a3) - (6 * a2) + (4 * a1) - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
