/*
 * A043306  Sum of all digits in all base-b representations for n, for 2 <= b
 *          <= n.
 * https://oeis.org/A043306
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b=2}^{n} (sum of digits of n in base b)
 * Range:     u8 n <= 33, u16 n <= 598, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be at least 2 by the caller. */
    seq_t total = 0;
    for (idx_t b = 2; b <= n; ++b) {
        idx_t m = n;
        seq_t sum = 0;                    /* digit sum in base b */
        while (m > 0) {
            sum += (seq_t)(m % b);        /* digit value */
            m /= b;
        }
        total += sum;
    }
    return total;
}
