/*
 * A174114  Even central polygonal numbers (A193868) divided by 2.
 * https://oeis.org/A174114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(4*m+3)+1, where * m = (n-1)/2 for odd n, * m = -(n/2)
 *            for even n. * Equivalent compact form: * a(2k-1) = 4*(k-1)^2 +
 *            3*(k-1) + 1, * a(2k) = k*(4*k-3) + 1
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {
        /* n is odd: m = (n-1)/2  (non‑negative) */
        seq_t m   = (seq_t)((n - 1) / 2);  /* m ≥ 0 */
        seq_t m2  = m * m;                /* m²          */
        /* 4*m² + 3*m + 1 */
        return m2 * 4 + m * 3 + 1;
    } else {
        /* n is even: k = n/2 (≥ 1),  a(2k) = k*(4k-3)+1 */
        seq_t k      = (seq_t)(n / 2);     /* k ≥ 1 */
        seq_t fourk  = k * 4;              /* 4k          */
        seq_t prod   = k * (fourk - 3);    /* k*(4k-3)    */
        return prod + 1;
    }
}
