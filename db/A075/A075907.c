/*
 * A075907  Fourth column of triangle A075499.
 * https://oeis.org/A075907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 560*a(n-2) + 3200*a(n-3) - 6144*a(n-4), with
 *            a(0)=1, a(1)=40, a(2)=1040, a(3)=22400
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)1040;
    if (n == 3) return (seq_t)22400;

    seq_t a0 = (seq_t)1;        /* a(n-4) */
    seq_t a1 = (seq_t)40;       /* a(n-3) */
    seq_t a2 = (seq_t)1040;     /* a(n-2) */
    seq_t a3 = (seq_t)22400;    /* a(n-1) */
    seq_t tmp;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute tmp = 40*a3 - 560*a2 + 3200*a1 - 6144*a0
           avoid negative intermediate by reordering. */
        tmp = (seq_t)40 * a3 + (seq_t)3200 * a1;
        tmp -= (seq_t)560 * a2;
        tmp -= (seq_t)6144 * a0;

        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = tmp;
    }
    return a3;
}
