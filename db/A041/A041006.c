/*
 * A041006  Numerators of continued fraction convergents to sqrt(6).
 * https://oeis.org/A041006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041006: Numerators of continued fraction convergents to sqrt(6)
    Formula: a(0) = 2, a(1) = 5,
             a(n) = C(n)*a(n-1) + a(n-2),   n >= 2
    where C(n) = 4 if n is even, else 2. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)5;

    seq_t p_prev2 = (seq_t)2; /* a(0) */
    seq_t p_prev1 = (seq_t)5; /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t coef = (i % 2 == 0) ? (seq_t)4 : (seq_t)2;
        seq_t p_new = coef * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_new;
    }
    return p_prev1;
}
