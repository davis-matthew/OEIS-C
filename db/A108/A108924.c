/*
 * A108924  a(n) = J(n)^2 + J(n+1)^2, with J(n) the Jacobsthal number
 *          A001045(n).
 * https://oeis.org/A108924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = J(n)^2 + J(n+1)^2 Recurrence: a(n) = 3*a(n-1) + 6*a(n-2)
 *            - 8*a(n-3) Initial values: a(0)=1, a(1)=2, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 2;
    if (n == 2u) return 10;

    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 2;   /* a(n-2) */
    seq_t a2 = 10;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t = a2 * 3 + a1 * 6;  /* 3*a(n-1) + 6*a(n-2) */
        seq_t s = a0 * 8;           /* 8*a(n-3) */
        seq_t ai = t - s;           /* a(n) = t - s */

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
