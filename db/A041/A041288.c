/*
 * A041288  Numerators of continued fraction convergents to sqrt(157).
 * https://oeis.org/A041288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) are numerators of the continued‐fraction convergents to
 *            sqrt(157)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 12, subsequent a(n) satisfy
   a(n) = A[n] * a(n-1) + a(n-2),   with a(-1) = 1, a(0) = 12
   where A[0] … A[16] is the period of the CF of sqrt(157):
   1,1,7,1,5,2,1,1,1,1,2,5,1,7,1,1,24   (length 17) */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)12;
    }

    /* Periodic CF coefficients after the integer part */
    static const seq_t period[17] = {
        (seq_t)1,  /* a1  */
        (seq_t)1,  /* a2  */
        (seq_t)7,  /* a3  */
        (seq_t)1,  /* a4  */
        (seq_t)5,  /* a5  */
        (seq_t)2,  /* a6  */
        (seq_t)1,  /* a7  */
        (seq_t)1,  /* a8  */
        (seq_t)1,  /* a9  */
        (seq_t)1,  /* a10 */
        (seq_t)2,  /* a11 */
        (seq_t)5,  /* a12 */
        (seq_t)1,  /* a13 */
        (seq_t)7,  /* a14 */
        (seq_t)1,  /* a15 */
        (seq_t)1,  /* a16 */
        (seq_t)24  /* a17 */
    };

    idx_t i;
    seq_t p_minus2 = (seq_t)1;   /* a(-1) */
    seq_t p_minus1 = (seq_t)12;  /* a(0) */
    seq_t p_curr;

    for (i = 1; i <= n; ++i) {
        seq_t ak = period[(i - 1) % 17];
        p_curr = ak * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }
    return p_minus1;
}
