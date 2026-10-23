/*
 * A041100  Numerators of continued fraction convergents to sqrt(58).
 * https://oeis.org/A041100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=7, a(1)=8.
   For n >= 2: a(n) = ((n-1)%7==6 ? 14 : 1) * a(n-1) + a(n-2).
   This comes from the numerators of the convergents to sqrt(58),
   whose continued fraction coefficients are [7; 1,1,1,1,1,1,14] repeating. */
seq_t a(idx_t n)
{
    seq_t p_prevprev = 0; /* p_{-2} */
    seq_t p_prev     = 1; /* p_{-1} */
    seq_t p = 0;          /* current numerator p_n */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i = (i == 0) ? 7
                    : (( (i - 1) % 7ULL) == 6ULL ? 14 : 1);
        p = a_i * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p;
    }
    return p;
}
