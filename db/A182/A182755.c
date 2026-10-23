/*
 * A182755  Expansion of (1+35*x)/(1-90*x^2).
 * https://oeis.org/A182755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 35 * 90^(k-1) (k ≥ 1) a(2k-1) = 90 ^(k-1) (k ≥ 1) Thus
 *            a(n) = 90^((n-1)/2) if n is odd a(n) = 35 * 90^(n/2-1) if n is
 *            even
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1) = 1.  Undefined for n < 1. */
    if (n == 0) return 0;

    if (n & 1)   /* Odd index n = 2k-1 */
    {
        idx_t k = (n - 1) / 2;
        seq_t result = 1;
        for (idx_t i = 0; i < k; ++i) {
            result *= 90;
        }
        return result;
    }
    else        /* Even index n = 2k */
    {
        idx_t k = n / 2;
        seq_t result = 35;          /* 35 * 90^(k-1), exp = k-1 */
        if (k > 1) {
            for (idx_t i = 0; i < k - 1; ++i) {
                result *= 90;
            }
        }
        return result;
    }
}
