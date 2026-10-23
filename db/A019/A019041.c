/*
 * A019041  Expansion of 1/((1-4x)(1-5x)(1-12x)).
 * https://oeis.org/A019041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 128*a(n-2) + 240*a(n-3) (n>2) Initial values:
 *            a(0)=1, a(1)=21, a(2)=313
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)313;

    seq_t a0 = (seq_t)1;   /* a(n‑3) */
    seq_t a1 = (seq_t)21;  /* a(n‑2) */
    seq_t a2 = (seq_t)313; /* a(n‑1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i)
    {
        an = (seq_t)21 * a2 - (seq_t)128 * a1 + (seq_t)240 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
