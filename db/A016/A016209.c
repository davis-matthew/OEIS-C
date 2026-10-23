/*
 * A016209  Expansion of 1/((1-x)(1-3x)(1-5x)).
 * https://oeis.org/A016209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) – 15*a(n-2) + 1 (a(0)=1, a(1)=9)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    /* Iterate forward using the linear recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)9;   /* a(1) */
    seq_t an;              /* a(i) for current i */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)8 * a1 - (seq_t)15 * a0 + (seq_t)1;
        a0 = a1;
        a1 = an;
    }
    return an;
}
