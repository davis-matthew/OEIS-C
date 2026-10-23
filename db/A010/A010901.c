/*
 * A010901  Pisot sequences E(4,7), P(4,7).
 * https://oeis.org/A010901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3), a(0)=4, a(1)=7, a(2)=12
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 76, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)12;

    /* Iteratively compute using the recurrence. */
    seq_t a0 = (seq_t)4;   /* a(n-3) */
    seq_t a1 = (seq_t)7;   /* a(n-2) */
    seq_t a2 = (seq_t)12;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = a2 + a2;  /* 2*a(n-1) */
        a3 -= a1;            /* - a(n-2) */
        a3 += a0;            /* + a(n-3) */

        /* Shift for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;               /* a(n) */
}
