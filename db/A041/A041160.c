/*
 * A041160  Numerators of continued fraction convergents to sqrt(90).
 * https://oeis.org/A041160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=9, a(1)=19, for n≥2: a(n) = (n%2==0?18:2)*a(n-1)+a(n-2) */
seq_t a(idx_t n)
{
    seq_t p_prev_prev = (seq_t)0;
    seq_t p_prev = (seq_t)1;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t ak;
        if (k == 0)
            ak = (seq_t)9;          /* a_0 = 9  */
        else if (k & 1U)
            ak = (seq_t)2;          /* odd k → 2  */
        else
            ak = (seq_t)18;         /* even k → 18 */

        seq_t p = ak * p_prev + p_prev_prev;   /* p_k = a_k p_{k-1} + p_{k-2} */

        if (k == n)
            return p;

        p_prev_prev = p_prev;
        p_prev = p;
    }

    return (seq_t)0;   /* unreachable */
}
