/*
 * A226508  a(n) = Sum_{i=3^n..3^(n+1)-1} i.
 * https://oeis.org/A226508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=33, a(n)=12*a(n-1)-27*a(n-2) for n≥2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)33;

    /* Compute the sequence iteratively from the bottom up */
    seq_t a0 = (seq_t)3;   /* a(0) */
    seq_t a1 = (seq_t)33;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = 12 * a1 - 27 * a0;  /* recurrence */
        a0 = a1;
        a1 = an;
    }

    return a1;  /* a(n) */
}
