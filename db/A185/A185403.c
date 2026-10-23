/*
 * A185403  a(n) = (7^n/n!^2) * Product_{k=0..n-1} (14k+3)*(14k+4).
 * https://oeis.org/A185403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7^n / (n!^2)) * Product_{k=0..n-1} (14k+3)*(14k+4)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) {
        /* Multiply by the factor for step i */
        seq_t temp = res * (seq_t)7;
        temp = temp * (seq_t)(14 * i + 3);
        temp = temp * (seq_t)(14 * i + 4);

        /* Divide by (i+1)^2 */
        seq_t denom = (seq_t)(i + 1);
        denom = denom * denom;

        res = temp / denom;
    }
    return res;
}
