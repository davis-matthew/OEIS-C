/*
 * A098293  Powers of 2 alternating with powers of 3.
 * https://oeis.org/A098293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = 2^k, a(2*k+1) = 3^k, for k >= 0
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even index -> power of 2 */
    if ((n & 1) == 0)
    {
        idx_t k = n >> 1;          /* k = n / 2 */
        seq_t res = 1;
        for (idx_t i = 0; i < k; ++i)
        {
            res *= (seq_t)2;
        }
        return res;
    }
    /* Odd index -> power of 3 */
    else
    {
        idx_t k = n >> 1;          /* floor(n/2) */
        seq_t result = 1;
        seq_t base = (seq_t)3;
        while (k > 0)
        {
            if (k & 1)
                result *= base;
            base *= base;
            k >>= 1;
        }
        return result;
    }
}
