/*
 * A106842  a(n) = (1 + n + n^2)^n.
 * https://oeis.org/A106842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 - n + 1)^(n-1) (offset 1, a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;               /* undefined for n = 0, but avoid overflow */
    }

    /* exponent k = n - 1 */
    idx_t k = n - 1;
    seq_t exp = (seq_t)k;

    /* base = n^2 - n + 1 */
    seq_t sn = (seq_t)n;
    seq_t base = sn * sn - sn + 1;

    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {
            base *= base;
        }
    }
    return result;
}
