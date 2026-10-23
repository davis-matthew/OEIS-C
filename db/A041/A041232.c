/*
 * A041232  Numerators of continued fraction convergents to sqrt(128).
 * https://oeis.org/A041232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=11; a(1)=34; for n>=2, a(n)=period[(n-1)%4]*a(n-1)+a(n-2)
   where period={3,5,3,22}. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;

    static const seq_t period[4] = {
        (seq_t)3,
        (seq_t)5,
        (seq_t)3,
        (seq_t)22
    };

    seq_t p_prev2 = (seq_t)1;          /* a(-1) */
    seq_t p_prev1 = (seq_t)11;         /* a(0) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ak   = period[(k-1) % 4];
        seq_t curr = ak * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = curr;
    }

    return p_prev1;
}
