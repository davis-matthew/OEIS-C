/*
 * A041604  Numerators of continued fraction convergents to sqrt(320).
 * https://oeis.org/A041604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(320).  
   The continued fraction of sqrt(320) is [17; overline{1,7,1,34}].  
   The numerators satisfy
        a(0) = 17, a(1) = 18, and for n ≥ 2
        a(n) = a_n * a(n‑1) + a(n‑2),
   where a_n = 1,7,1,34 repeating (i.e., a_n = [1,7,1,34][(n-1)%4]). */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;
    if (n == 1)
        return (seq_t)18;

    static const seq_t period[4] = {(seq_t)1, (seq_t)7, (seq_t)1, (seq_t)34};

    seq_t p_prev2 = (seq_t)17; /* a(0) */
    seq_t p_prev1 = (seq_t)18; /* a(1) */
    seq_t p = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = period[(i - 1) % 4];
        p = ai * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
