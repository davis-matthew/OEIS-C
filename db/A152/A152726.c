/*
 * A152726  a(n) = n^7 - (n-1)^7 + (n-2)^7 - ... + ((-1)^n)*0^7.
 * https://oeis.org/A152726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 - (n-1)^7 + (n-2)^7 - ... + ((-1)^n)*0^7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 623, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pos = 0;   /* sum of terms with sign +1  */
    seq_t neg = 0;   /* sum of terms with sign -1  */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;

        /* kk^7 = kk^3 * kk^4 */
        seq_t k2 = kk * kk;      /* kk^2   */
        seq_t k3 = k2 * kk;      /* kk^3   */
        seq_t k4 = k2 * k2;      /* kk^4   */
        seq_t k7 = k3 * k4;      /* kk^7   */

        /* parity of (n - k) determines the sign of this term */
        if (((n - k) & 1) == 0)
            pos += k7;
        else
            neg += k7;
    }
    return pos - neg;
}
