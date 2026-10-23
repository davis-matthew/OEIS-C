/*
 * A019671  Expansion of 1/((1-4x)(1-8x)(1-10x)).
 * https://oeis.org/A019671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 80*a(n-2) + 4^n (a(0)=1, a(1)=22)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)22;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)22;  /* a(n-1) */

    seq_t pow4 = (seq_t)4; /* 4^1 */

    for (idx_t i = 2u; i <= n; ++i)
    {
        pow4 *= (seq_t)4;                 /* now pow4 = 4^i */
        seq_t ai = 18 * a1 - 80 * a0 + pow4;
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
