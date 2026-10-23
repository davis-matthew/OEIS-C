/*
 * A042644  Numerators of continued fraction convergents to sqrt(852).
 * https://oeis.org/A042644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to sqrt(852). Recurrence:
 *            a(0) = 29, a(n) = c_n * a(n-1) + a(n-2) for n >= 1, where the
 *            continued‑fraction terms c_n repeat with period 8:
 *            {5,3,2,4,2,3,5,58}
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;
    static const seq_t period[8] = {5,3,2,4,2,3,5,58};
    seq_t p_nm2 = 1;      /* a(-1) */
    seq_t p_nm1 = 29;     /* a(0)  */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = period[(i - 1) & 7];
        seq_t p = t * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p;
    }
    return p_nm1;
}
