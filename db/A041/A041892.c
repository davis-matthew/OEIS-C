/*
 * A041892  Numerators of continued fraction convergents to sqrt(468).
 * https://oeis.org/A041892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=21, a(1)=22; for n≥2,
 * a(n)=c_n*a(n-1)+a(n-2) with c_n cycling 1,1,1,2,1,1,1,42 (c_8=42). */
seq_t a(idx_t n)
{
    static const unsigned int period[8] = {1,1,1,2,1,1,1,42};
    seq_t p0 = 0; /* p_{-2} */
    seq_t p1 = 1; /* p_{-1} */
    seq_t p;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)21;
        } else {
            a_k = (seq_t)period[(k - 1) & 7];
        }
        p = a_k * p1 + p0;
        p0 = p1;
        p1 = p;
    }
    return p1;
}
