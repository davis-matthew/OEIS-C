/*
 * A163604  a(n) = ((3+2*sqrt(2))*(4+sqrt(2))^n +
 *          (3-2*sqrt(2))*(4-sqrt(2))^n)/2.
 * https://oeis.org/A163604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n‑1) – 14*a(n‑2), with a(0)=3, a(1)=16
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)16;

    seq_t a0 = (seq_t)3;   /* a(0) */
    seq_t a1 = (seq_t)16;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 8*a(i-1) – 14*a(i-2) */
        an = a1 * (seq_t)8 - a0 * (seq_t)14;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
