/*
 * A041662  Numerators of continued fraction convergents to sqrt(350).
 * https://oeis.org/A041662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction numerators for sqrt(350)
   a(0) = 18
   Let c_k be the k‑th term of the continued fraction coefficients
   after the integer part: 1, 2, 2, 2, 1, 36 repeating.
   Then for n >= 2:
       a(n) = c_n * a(n-1) + a(n-2)
*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;  /* a(0) */
    /* Periodic continued fraction coefficients */
    static const seq_t period[6] = {
        (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)1, (seq_t)36
    };
    seq_t p_prevprev = (seq_t)1; /* p_-1 */
    seq_t p_prev     = (seq_t)18;/* p_0 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai = period[(i - 1) % 6];
        seq_t p_curr = ai * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_curr;
    }
    return p_prev; /* a(n) */
}
