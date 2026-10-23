/*
 * A040753  Continued fraction for sqrt(781).
 * https://oeis.org/A040753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(781) */
seq_t a(idx_t n)
{
    /* a0 = floor(sqrt(781)) = 27 */
    const seq_t D   = 781;
    const seq_t a0  = 27;

    if (n == 0) return a0;

    seq_t m = 0;   /* m0  */
    seq_t d = 1;   /* d0  */
    seq_t a_curr = a0; /* a0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* m_{i} = d_{i-1} * a_{i-1} - m_{i-1} */
        m = d * a_curr - m;
        /* d_{i} = (D - m_{i}^2) / d_{i-1} */
        d = (D - m * m) / d;
        /* a_{i} = floor((a0 + m_{i}) / d_{i}) */
        a_curr = (a0 + m) / d;
    }
    return a_curr;
}
