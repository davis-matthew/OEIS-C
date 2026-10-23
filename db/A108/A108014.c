/*
 * A108014  Expansion of (x^2-2*x)/(x^4-x^2+2*x-1).
 * https://oeis.org/A108014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=2, a(2)=3, a(3)=4; a(n)=2*a(n-1)-a(n-2)+a(n-4) */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)3;
    if (n == 3u) return (seq_t)4;

    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)2;   /* a(n-3) */
    seq_t a2 = (seq_t)3;   /* a(n-2) */
    seq_t a3 = (seq_t)4;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = a3 * (seq_t)2 - a2 + a0;  /* 2*a(n-1)-a(n-2)+a(n-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
