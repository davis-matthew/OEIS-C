/*
 * A041610  Numerators of continued fraction convergents to sqrt(323).
 * https://oeis.org/A041610
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

/* Numerators of continued fraction convergents to sqrt(323).
   Recurrence: a(0)=17, a(1)=18.
   For n>=2: a(n) = (if n is even then 34 else 1) * a(n-1) + a(n-2). */
seq_t a(idx_t n)
{
    seq_t a_minus2 = (seq_t)0;  /* a(-2) */
    seq_t a_minus1 = (seq_t)1;  /* a(-1) */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)17;
        else if (i == 1)
            coeff = (seq_t)1;
        else
            coeff = ( (i & 1) == 0 ) ? (seq_t)34 : (seq_t)1; /* even i => 34, odd => 1 */

        seq_t a_cur = coeff * a_minus1 + a_minus2;
        if (i == n)
            return a_cur;
        a_minus2 = a_minus1;
        a_minus1 = a_cur;
    }
    /* Should never reach here */
    return (seq_t)0;
}
