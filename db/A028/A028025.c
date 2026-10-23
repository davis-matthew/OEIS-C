/*
 * A028025  Expansion of 1/((1-3x)*(1-4x)*(1-5x)*(1-6x)).
 * https://oeis.org/A028025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 119*a(n-2) + 342*a(n-3) - 360*a(n-4) (n >= 4)
 *            Initial values: a(0)=1, a(1)=18, a(2)=205, a(3)=1890
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)18;
    if (n == 2U) return (seq_t)205;
    if (n == 3U) return (seq_t)1890;

    seq_t t0 = (seq_t)1;    /* a(n-4) */
    seq_t t1 = (seq_t)18;   /* a(n-3) */
    seq_t t2 = (seq_t)205;  /* a(n-2) */
    seq_t t3 = (seq_t)1890; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)18 * t3
                   - (seq_t)119 * t2
                   + (seq_t)342 * t1
                   - (seq_t)360 * t0;

        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
