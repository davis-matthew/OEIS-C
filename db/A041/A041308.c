/*
 * A041308  Numerators of continued fraction convergents to sqrt(167).
 * https://oeis.org/A041308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041308: Numerators of continued fraction convergents to sqrt(167)
   Recurrence: a(0)=12, a(1)=13,
   for n≥1: a(n) = c[n] * a(n−1) + a(n−2)
   where c = [1,11,1,24] repeats with period 4. */
seq_t a(idx_t n)
{
    /* periodic partial quotients for sqrt(167): 1, 11, 1, 24 */
    static const seq_t coeff[4] = { 1, 11, 1, 24 };

    if (n == 0) 
        return 12;

    seq_t p_minus2 = 1; /* a(-1) */
    seq_t p_minus1 = 12; /* a(0) */
    seq_t p_curr   = 0;   /* placeholder for a(n) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t c = coeff[(i - 1) & 3]; /* period of 4 */
        p_curr = c * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }
    return p_curr;
}
