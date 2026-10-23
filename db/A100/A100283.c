/*
 * A100283  a(n) = floor(p*(n+1)) - floor(p*(n)) - 1 where p = Padovan plastic
 *          number = 1.324718... (cf. A060006).
 * https://oeis.org/A100283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(p*(n+1)) - floor(p*n) - 1 where p ≈
 *            1.324717957244746... (Padovan plastic number)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 10^12 scaling of the fractional part of p */
    static const seq_t FP = 324717957244ULL; /* floor(p*10^12) - 10^12 */
    static const seq_t Q  = 1000000000000ULL; /* 10^12 */

    seq_t f = 0;
    for (idx_t i = 0; i < n; ++i) {
        f += FP;
        if (f >= Q) f -= Q;           /* f = (f + FP) mod Q   */
    }
    /* a(n) is 1 when the increment from floor(p*n) to floor(p*(n+1))
       exceeds 1, i.e., when (f + FP) >= Q.                      */
    return (f + FP >= Q) ? 1 : 0;
}
