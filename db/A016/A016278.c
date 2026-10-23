/*
 * A016278  Expansion of g.f. 1/((1-2x)(1-3x)(1-9x)).
 * https://oeis.org/A016278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 51*a(n-2) + 54*a(n-3); a(0)=1, a(1)=14,
 *            a(2)=145
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)145;

    /* iterative recurrence using only three temporaries */
    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)14;  /* a(n-2)  */
    seq_t a2 = (seq_t)145; /* a(n-1)  */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = 14 * a2 + 54 * a0 - 51 * a1;  /* a(n) = 14*a(n-1) - 51*a(n-2) + 54*a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
