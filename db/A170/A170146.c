/*
 * A170146  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*a(n-1) + 39*a(n-2) + ... + 39*a(n-37) - 780*a(n-38) (n
 *            >= 38) with initial values: a(0) = 1 a(1) = 41 a(2) = 1640 a(3)
 *            = 65600 a(4) = 2624000 (the remaining initial terms up to a(38)
 *            follow from the generating function given in the OEIS entry)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == 0)
        return (seq_t)1;

    /* buffer holding the last 38 values a(k) */
    seq_t buf[38] = {0};
    buf[0] = (seq_t)1;          /* a(0) */

    /* iterate from 1 up to n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute sum_{j=1}^{37} a(i-j)   (only for existing indices) */
        seq_t sum = 0;
        for (idx_t j = 1; j <= 37; ++j) {
            if (i >= j) {
                sum += buf[(i - j) % 38];
            }
        }

        /* term = 39 * sum */
        seq_t term = 39 * sum;

        /* Add p_i from the numerator of the generating function */
        seq_t p = 0;
        if (i <= 37)
            p = 2;          /* numerator coefficients for t^1 … t^37 */
        else if (i == 38)
            p = 1;          /* coefficient for t^38 */

        /* Subtract 780 * a(i-38) if i >= 38 */
        seq_t sub = 0;
        if (i >= 38) {
            sub = 780 * buf[(i - 38) % 38];
        }

        /* Compute a(i) */
        seq_t val = term + p - sub;

        /* store the new value */
        buf[i % 38] = val;
    }

    /* the desired value is stored at position n % 38 */
    return buf[n % 38];
}
