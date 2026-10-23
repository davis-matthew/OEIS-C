/*
 * A195913  The denominator in a fraction expansion of log(2)-Pi/8.
 * https://oeis.org/A195913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(3*k) = (4*k-1)*(4*k) a(3*k+1) = (4*k+1)*(4*k+2) a(3*k+2) =
 *            (4*k+1)*(4*k+3) (k>=0)
 * Range:     u8 n <= 12, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, return 0 */
    idx_t mod = n % 3;
    switch (mod) {
    case 0: /* n = 3*k, k >= 1 */
    {
        idx_t k = n / 3;
        seq_t a1 = (seq_t)(4 * k - 1);
        seq_t a2 = (seq_t)(4 * k);
        return a1 * a2;
    }
    case 1: /* n = 3*k+1, k >= 0 */
    {
        idx_t k = (n - 1) / 3;
        seq_t a1 = (seq_t)(4 * k + 1);
        seq_t a2 = (seq_t)(4 * k + 2);
        return a1 * a2;
    }
    case 2: /* n = 3*k+2, k >= 0 */
    {
        idx_t k = (n - 2) / 3;
        seq_t a1 = (seq_t)(4 * k + 1);
        seq_t a2 = (seq_t)(4 * k + 3);
        return a1 * a2;
    }
    default:
        return 0; /* unreachable */
    }
}
