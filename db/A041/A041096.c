/*
 * A041096  Numerators of continued fraction convergents to sqrt(56).
 * https://oeis.org/A041096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(56).
   Recurrence:  p_{-2} = 0,  p_{-1} = 1.
                a_0 = 7,  a_{k} = 2  if k is odd,
                                     14 if k is even (k ≥ 2).
                For k ≥ 0:   p_k = a_k * p_{k-1} + p_{k-2}.
   a(n) is the required term (a(0) = 7). */

seq_t a(idx_t n)
{
    seq_t p_nm2 = 0;      /* p_{k-2}   */
    seq_t p_nm1 = 1;      /* p_{k-1}   */
    seq_t p = 0;          /* current p_k  */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0)
            a_k = 7;
        else
            a_k = (k & 1) ? 2 : 14;   /* odd: 2, even: 14 */

        p = a_k * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p;
    }

    return p_nm1;
}
